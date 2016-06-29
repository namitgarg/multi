<?php

/**
 * @file
 * Stores the code for export process.
 */

/**
 * Gets the list of  Content Type for Content type dropdown.
 */
function metatag_import_export_csv_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = t("@typename", array('@typename' => $values1->name));
    $func_content_type[$type_machine_name] = $type_human_name;
  }
    $default_type = array('all' => 'All');
  $content_type = array_merge($default_type, $func_content_type);
  return $content_type;
}

/**
 * Form for exporting Meta tags.
 */
function metatag_import_export_csv_download_form($form, &$form_state) {
  $content_type = metatag_import_export_csv_type_list();
  $form['content_type'] = array(
    '#type' => 'select',
    '#options' => $content_type,
    '#title' => t("Content type"),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#default_value' => $content_type['all'],
    '#description' => t("Select the Content type"),
  );
  $form['export'] = array(
    '#type' => 'submit',
    '#value' => t("Export"),
    '#submit' => array('metatag_import_export_csv_download_submit'),
  );
  return $form;
}

/**
 * Submit Handler for Export.
 */
function metatag_import_export_csv_download_submit($form, &$form_state) {
  $type = $form_state['values']['content_type'];
  $file_path="private://metatag_import_export_csv/";
  metatag_import_export_csv_download_nid($type,$file_path);
}

/**
 * Gets the list of nid according to content type.
 */
function metatag_import_export_csv_download_nid($type,$file_path) {
  $query = db_select('node', 'n')
        ->fields('n', array('nid'));
  if ($type != 'all') {
    $query->condition('type', $type);
  }
  elseif ($type = 'all') {
    $type = metatag_import_export_csv_type_list();
    $type = array_keys($type);
    $query->condition('type', ($type), 'IN');
  }
  $results = $query->execute()->fetchAll();
  foreach ($results as $values) {
    $nid_list[] = $values->nid;
  }
    metatag_import_export_csv_process_csv($nid_list,$file_path);
}

/**
 * Creates CSV file and downloads it to system.
 */
function metatag_import_export_csv_process_csv($nid_list,$file_path) {
    $filename = 'metatag_import_export_csv.csv';
    $file_url = $file_path . $filename;
  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
  
  if (!file_prepare_directory($file_path, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Unable to create directory in file system. Check permissions and try again.'), 'error');
    return;
  }
  $file = file_save_data('', $file_url,FILE_EXISTS_REPLACE);

  if (!$file) {
    drupal_set_message(t('Unable to write to file system. Check permissions and try again.'), 'error');
    return;
  }
  $fp = fopen($file->uri, 'w');
  if (!$fp) {
    drupal_set_message(t('Unable open file for writing. Check permissions and try again.'), 'error');
    return;
  }
  $header = array('Node Title','Path', ' Meta Title', 'Meta Description', 'Meta Abstract', ' Meta Keywords');
        fputcsv($fp, $header);

  foreach ($nid_list as $keys => $values) {
    $node = node_load($values);
    $node_title=$node->title;
    $path = "node/" . $values;
    $meta_title = (isset($node->metatags[$node->language]['title']['value']) ? $node->metatags[$node->language]['title']['value'] : "");
    $meta_description = (isset($node->metatags[$node->language]['description']['value']) ? $node->metatags[$node->language]['description']['value'] : "");
    $meta_abstract = (isset($node->metatags[$node->language]['abstract']['value']) ? $node->metatags[$node->language]['abstract']['value'] : "");
    $meta_keywords = (isset($node->metatags[$node->language]['keywords']['value']) ? $node->metatags[$node->language]['keywords']['value'] : "");
    if (!empty($meta_title) || !empty($meta_description) || !empty($meta_abstract) || !empty($meta_keywords)) {  
      $row = array($node_title,$path, $meta_title, $meta_description, $meta_abstract, $meta_keywords);
     file_put_contents($file_url,$row,FILE_APPEND);
       fputcsv($fp, $row);
    }
  }

  fclose($fp);
  $file->filesize = filesize($file->uri);
  file_save($file);
  if(drupal_is_cli())
  {
     return($file);
  }
  file_transfer($file->uri, array(
      'Content-Type' => 'text/csv',
      'Content-Disposition' => 'attachment; filename="' . $filename . '"',
      'Content-Length' => filesize($file->uri)));
}
