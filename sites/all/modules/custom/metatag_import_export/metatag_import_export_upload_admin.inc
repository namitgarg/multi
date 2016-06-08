<?php

/**
 * @file
 * Stores the code for import Process.
 */

/**
 * Checks CSV for Html and Php.
 *
 * @return bool
 *   True if fields contains No Html or Php.
 */
function metatag_import_export_xss_check($title, $description, $abstract, $keywords) {
  $checked_title = strip_tags($title);
  $checked_description = strip_tags($description);
  $checked_abstract = strip_tags($abstract);
  $checked_keywords = strip_tags($keywords);
  if (($checked_title == $title) && ($checked_description == $description) && ($checked_abstract == $abstract) && ($checked_keywords == $keywords)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Form for Export operation.
 */
function metatag_import_export_upload_form($form, &$form_state) {
  metatag_import_export_getpath();
  $form['upload_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload File'),
    '#required' => TRUE,
    '#description' => t('Allowed Extensions: csv'),
    '#upload_location' => 'public://metatag_import_export/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('csv'),
      'file_validate_size' => array(1 * 1024 * 1024),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Import"),
    '#submit' => array('metatag_import_export_form_submit'),
  );
  return $form;
}

/**
 * Submmit Handler for Import Operation.
 */
function metatag_import_export_form_submit($form, &$form_state) {
  $flag = 1;
  $file = file_load($form_state['values']['upload_file']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  $fileurl = file_create_url($file->uri);
  $handle = fopen($fileurl, "r");
  fgetcsv($handle);
  while (!feof($handle)) {
    $data = fgetcsv($handle);
    $path[] = $data[0];
    $title[] = $data[1];
    $description[] = $data[2];
    $abstract[] = $data[3];
    $keywords[] = $data[4];
    $xsscheck = metatag_import_export_xss_check($data[0], $data[1], $data[2], $data[3], $data[4]);
    if (empty($xsscheck)) {
      $flag = 0;
    }
  }
  if ($flag == 1) {
    $batch = array(
      'operations' => array(),
      'finished' => 'metatag_import_export_finish_batch',
      'title' => t("Processing Nodes for importing Metatags"),
      'init_message' => t("Starting Metatag Import Process"),
      'progress_message' => t("Processed @current out of @total Nodes"),
      'file' => drupal_get_path('module', 'metatag_import_export') . '/metatag_import_export_upload_admin.inc',
      'error_message' => t("Metatag import process has encountered an error."),
    );
    foreach ($path as $keys => $values) {
      $batch['operations'][] = array('metatag_import_export_start_batch', array(
        $values,
        $title[$keys],
        $description[$keys],
        $abstract[$keys],
        $keywords[$keys],
        $keys,
        ));
    }
    batch_set($batch);
    // The path to redirect when batch process is completed.
    batch_process('admin/config/search/metatags/import');
  }
  else {
    drupal_set_message(t("CSV contains HTML and PHP."), 'error');
  }
}

/**
 * Batch Operation Callback.
 */
function metatag_import_export_start_batch($path, $title, $description, $abstract, $keywords, $keys, &$context) {
  $row_num = $keys + 2;
  $nid_db = metatag_import_export_getnid();
  preg_match("/node/", $path, $path_match);
  $cur_nid = explode("/", $path)[1];
  $flag = 0;
  if (!empty($path_match)) {
    if (in_array($path, $nid_db)) {
      metatag_import_export_node_processing($cur_nid, $title, $description, $abstract, $keywords, $keys);
      drupal_set_message(t("%path  Metatag on row %row_num updated Successfully", array('%path ' => $path, '%row_num' => $row_num)));
    }
    else {
      drupal_set_message(t("%path  Metatag on row %row_num does not exists", array('%path' => $path, '%row_num' => $row_num)), 'error');
    }
  }
  else {
    $pathdb = metatag_import_export_getpath();
    foreach ($pathdb['alias_path'] as $keys => $values) {
      $path_url[] = $values;
      $path_nid[] = $pathdb['source'][$keys];
    }
    $path_search = array_keys($path_url, $path)[0];
    if (!empty($path_nid[$path_search])) {
      metatag_import_export_node_processing($path_nid[$path_search], $title, $description, $abstract, $keywords, $keys);
      drupal_set_message(t("%path  Metatag on row %row_num updated Successfully", array('%path ' => $path, '%row_num' => $row_num)));
    }
    else {
      drupal_set_message(t("%path  Metatag on row %row_num does not exists", array('%path' => $path, '%row_num' => $row_num)), 'error');
    }
  }
}

/**
 * Batch 'finished' callback for Import Operation.
 */
function metatag_import_export_finish_batch($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t("Finished Importing Metatags"));
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t("An error occurred while processing @operation with arguments : @args", array(
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        )));
  }
}

/**
 * Gets nid of all nodes.
 */
function metatag_import_export_getnid() {
  $query = db_select('node', 'n')
        ->fields('n', array('nid'));
  $results = $query->execute()->fetchAll();
  foreach ($results as $values) {
    $nid_list[] = "node/" . $values->nid;
  }
  return ($nid_list);
}

/**
 * Get URl Alias from database.
 */
function metatag_import_export_getpath() {
  $query2 = db_select('url_alias', 'ua')
      ->fields('ua', array('source', 'alias'));
  $results2 = $query2->execute()->fetchAll();
  foreach ($results2 as $values) {
    $path['source'][] = explode("/", $values->source)[1];
    $path['alias_path'][] = $values->alias;
  }
  return ($path);
}

/**
 * Changes the Metatag for the Nodes.
 */
function metatag_import_export_node_processing($cur_nid, $title, $description, $abstract, $keywords, $keys) {
  $node = node_load($cur_nid);
  if (!empty($title)) {
    $node->metatags[$node->language]['title']['value'] = $title;
  }
  if (!empty($description)) {
    $node->metatags[$node->language]['description']['value'] = $description;
  }
  if (!empty($abstract)) {
    $node->metatags[$node->language]['abstract']['value'] = $abstract;
  }
  if (!empty($keywords)) {
    $node->metatags[$node->language]['keywords']['value'] = $keywords;
  }
  $node->revision = 1;
  $node->log = "Modified by Metatag Import Export Module.";
  node_save($node);
}
