<?php

/**
 * @file
 * Stores the code for export process.
 */

/**
 * Gets the list of  Content Type for Content type dropdown.
 */
function metatag_import_export_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = t("@typename", array('@typename' => $values1->name));
    $func_content_type[$type_machine_name] = $type_human_name;
  }
  return $func_content_type;
}

/**
 * Form for exporting Meta tags.
 */
function metatag_import_export_download_form($form, &$form_state) {
  $content_type = metatag_import_export_type_list();
  // All Content type will be selected by default.
  $default_type = array('all' => 'All');
  // To Merge list of individual content type with all option.
  $content_type = array_merge($default_type, $content_type);
  $form['content_type'] = array(
    '#type' => 'select',
    '#options' => $content_type,
    '#title' => t("Content type"),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#default_value' => $default_type,
    '#description' => t("Select the Content type"),
  );
  $form['export'] = array(
    '#type' => 'submit',
    '#value' => t("Export"),
    '#submit' => array('metatag_import_export_download_submit'),
  );
  return $form;
}

/**
 * Submit Handler for Export.
 */
function metatag_import_export_download_submit($form, &$form_state) {
  $type = $form_state['values']['content_type'];
  $nid_list = metatag_import_export_download_nid($type);
  $filename = 'metatag_import_export' . time() . '.csv';
  metatag_import_export_process_csv($nid_list, $filename);
}

/**
 * Gets the list of nid according to content type.
 */
function metatag_import_export_download_nid($type) {
  $query = db_select('node', 'n')
        ->fields('n', array('nid'));
  if ($type != 'all') {
    $query->condition('type', $type);
  }
  elseif ($type = 'all') {
    $type = metatag_import_export_type_list();
    $type = array_keys($type);
    $query->condition('type', ($type), 'IN');
  }
  $results = $query->execute()->fetchAll();
  foreach ($results as $values) {
    $nid_list[] = $values->nid;
  }
  return ($nid_list);
}

/**
 * Creates CSV file and downloads it to system.
 */
function metatag_import_export_process_csv($nid_list, $filename = 'metatag_import_export.csv') {
  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
  $path = "private://metatag_import_export/";
  $file_url = $path . $filename;
  if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Unable to create directory in file system. Check permissions and try again.'), 'error');
    return;
  }
  $file = file_save_data('', $path . $filename);
  if (!$file) {
    drupal_set_message(t('Unable to write to file system. Check permissions and try again.'), 'error');
    return;
  }
  $fp = fopen($file->uri, 'w');
  if (!$fp) {
    drupal_set_message(t('Unable open file for writing. Check permissions and try again.'), 'error');
    return;
  }
  $header = array('Path', 'Title', 'Description', 'Abstract', 'Keywords');
  fputcsv($fp, $header);
  foreach ($nid_list as $keys => $values) {
    $node = node_load($values);
    $path = "node/" . $values;
    $title = (isset($node->metatags[$node->language]['title']['value']) ? $node->metatags[$node->language]['title']['value'] : "");
    $description = (isset($node->metatags[$node->language]['description']['value']) ? $node->metatags[$node->language]['description']['value'] : "");
    $abstract = (isset($node->metatags[$node->language]['abstract']['value']) ? $node->metatags[$node->language]['abstract']['value'] : "");
    $keywords = (isset($node->metatags[$node->language]['keywords']['value']) ? $node->metatags[$node->language]['keywords']['value'] : "");
    if (!empty($title) || !empty($description) || !empty($abstract) || !empty($keywords)) {
      $meta_csv['path'][] = "node/" . $values;
      $meta_csv['title'][] = $title;
      $meta_csv['description'][] = $description;
      $meta_csv['abstract'][] = $abstract;
      $meta_csv['keywords'][] = $node->metatags[$node->language]['keywords']['value'];
      $row = array($path, $title, $description, $abstract, $keywords);
      fputcsv($fp, $row);
    }
  }
  fclose($fp);
  $file->filesize = filesize($file->uri);
  file_save($file);
  file_transfer($file->uri, array(
      'Content-Type' => 'text/csv',
      'Content-Disposition' => 'attachment; filename="' . $filename . '"',
      'Content-Length' => filesize($file->uri)));
}
