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
function metatag_import_export_csv_xss_check($node_title, $path, $meta_title, $meta_description, $meta_abstract, $meta_keywords) {
    $checked_node_title = strip_tags($node_title);
    $checked_path = strip_tags($path);
    $checked_meta_title = strip_tags($meta_title);
    $checked_meta_description = strip_tags($meta_description);
    $checked_meta_abstract = strip_tags($meta_abstract);
    $checked_meta_keywords = strip_tags($meta_keywords);
    if (($checked_node_title == $node_title) && ($checked_path == $path) && ($checked_meta_title == $meta_title) && ($checked_meta_description == $meta_description) && ($checked_meta_abstract == $meta_abstract) && ($checked_meta_keywords == $meta_keywords)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/**
 * Form for Export operation.
 */
function metatag_import_export_csv_upload_form($form, &$form_state) {
    metatag_import_export_csv_getpath();
    $form['upload_file'] = array(
      '#type' => 'managed_file',
      '#title' => t('Upload File'),
      '#required' => TRUE,
      '#description' => t('Allowed Extensions: csv'),
      '#upload_location' => 'public://metatag_import_export_csv/',
      '#upload_validators' => array(
        'file_validate_extensions' => array('csv'),
        'file_validate_size' => array(1 * 1024 * 1024),
      ),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Import"),
      '#submit' => array('metatag_import_export_csv_form_submit'),
    );
    return $form;
}

/**
 * Submmit Handler for Import Operation.
 */
function metatag_import_export_csv_form_submit($form, &$form_state) {
    $flag = 1;
    $file = file_load($form_state['values']['upload_file']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $fileurl = file_create_url($file->uri);
    $handle = fopen($fileurl, "r");
    fgetcsv($handle);
    while (!feof($handle)) {
        $data = fgetcsv($handle);
        $title[] = $data[0];
        $path[] = $data[1];
        $meta_title[] = $data[2];
        $meta_description[] = $data[3];
        $meta_abstract[] = $data[4];
        $meta_keywords[] = $data[5];
        $xsscheck = metatag_import_export_csv_xss_check($data[0], $data[1], $data[2], $data[3], $data[4], $data[5]);
        if (empty($xsscheck)) {
            $flag = 0;
        }
    }
    if ($flag == 1) {
        $batch = array(
          'operations' => array(),
          'finished' => 'metatag_import_export_csv_finish_batch',
          'title' => t("Processing Nodes for importing Metatags"),
          'init_message' => t("Starting Metatag Import Process"),
          'progress_message' => t("Processed @current out of @total Nodes"),
          'file' => drupal_get_path('module', 'metatag_import_export_csv') . '/metatag_import_export_csv_upload_admin.inc',
          'error_message' => t("Metatag import process has encountered an error."),
        );
        foreach ($path as $keys => $values) {
            $batch['operations'][] = array('metatag_import_export_csv_start_batch', array(
                $values,
                $meta_title[$keys],
                $meta_description[$keys],
                $meta_abstract[$keys],
                $meta_keywords[$keys],
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
function metatag_import_export_csv_start_batch($path, $meta_title, $meta_description, $meta_abstract, $meta_keywords, $keys, &$context) {
    $row_num = $keys + 2;
    $nid_db = metatag_import_export_csv_getnid();
    preg_match("/node/", $path, $path_match);
    $cur_nid = explode("/", $path)[1];
    $flag = 0;
    if (!empty($path_match)) {
        if (in_array($path, $nid_db)) {
            metatag_import_export_csv_node_processing($cur_nid, $meta_title, $meta_description, $meta_abstract, $meta_keywords, $keys);
            drupal_set_message(t("%path  Metatag on row %row_num updated Successfully", array('%path ' => $path, '%row_num' => $row_num)));
        }
        else {
            drupal_set_message(t("%path  Metatag on row %row_num does not exists", array('%path' => $path, '%row_num' => $row_num)), 'error');
        }
    }
    else {
        $pathdb = metatag_import_export_csv_getpath();
        foreach ($pathdb['alias_path'] as $keys => $values) {
            $path_url[] = $values;
            $path_nid[] = $pathdb['source'][$keys];
        }
        $path_search = array_keys($path_url, $path)[0];
        if (!empty($path_nid[$path_search])) {
            metatag_import_export_csv_node_processing($path_nid[$path_search], $meta_title, $meta_description, $meta_abstract, $meta_keywords, $keys);
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
function metatag_import_export_csv_finish_batch($success, $results, $operations) {
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
function metatag_import_export_csv_getnid() {
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
function metatag_import_export_csv_getpath() {
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
function metatag_import_export_csv_node_processing($cur_nid, $meta_title, $meta_description, $meta_abstract, $meta_keywords, $keys) {
    $node = node_load($cur_nid);
    if (!empty($meta_title)) {
        $node->metatags[$node->language]['title']['value'] = $meta_title;
    }
    if (!empty($meta_description)) {
        $node->metatags[$node->language]['description']['value'] = $meta_description;
    }
    if (!empty($meta_abstract)) {
        $node->metatags[$node->language]['abstract']['value'] = $meta_abstract;
    }
    if (!empty($meta_keywords)) {
        $node->metatags[$node->language]['keywords']['value'] = $meta_keywords;
    }
    $node->revision = 1;
    $node->log = "Modified by Metatag Import Export Module.";
    node_save($node);
}
