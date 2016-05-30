<?php

/**
 * @file
 * All the search and replace operatons are performed here.
 */

/**
 * Checks field for Html and Php.
 *
 * @return bool
 *   True if fields contains No Html or Php.
 */
function metatag_bulk_uploader_xss_check($data) {
  $new_data = strip_tags($data);
  if ($data == $new_data) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}


/**
 * Defines Main form for the module to Search and Replace.
 */
function metatag_bulk_uploader_form($form, &$form_state) {
  $form['search_box'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t("Search"),
    '#description' => t("Enter the text to be searched for"),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Submit"),
    '#validate' => array('metatag_bulk_uploader_form_search_validate'),
    '#submit' => array('metatag_bulk_uploader_form_search_submit'),
  );
    return $form;
}
/**
 * Form Validation for Search Operation.
 */
function metatag_bulk_uploader_form_search_validate($form, &$form_state) {
  $output = metatag_bulk_uploader_xss_check($form_state['values']['search_box']);
  if (!$output) {
    form_set_error('search_box', t("Do not Enter any HTML or Php tags"));
  }
}

/**
 * Submmit Handler for Search Operation.
 */
function metatag_bulk_uploader_form_search_submit($form, &$form_state) {
  $opn = 'search';
  metatag_bulk_uploader_search_operation($form, $form_state, $opn);
}


function metatag_bulk_uploader_search_operation($form, &$form_state, $opn) {
   $batch = array(
    'operations' => array(),
    'finished' => 'metatag_bulk_uploader_finish_batch_search',
    'title' => t("Processing  All Node For Searching"),
    'init_message' => t("Starting Metatag Search Operation"),
    'progress_message' => t("Processed @current out of @total Nodes"),
    'file' => drupal_get_path('module', 'metatag_bulk_uploader') . '/metatag_bulk_uploader_admin.inc',
    'error_message' => t("MetaTag Module has encountered an error."),
  );

    foreach ($results as $values) {
      $batch['operations'][] = array('metatag_bulk_uploader_start_batch_search', array($values->nid));
    }
    batch_set($batch);
    // The path to redirect when batch process is completed.
    batch_process('admin/config/search/metatags/replace');
}

/**
 * Batch Operation Callback.
 *
 */
function metatag_bulk_uploader_start_batch_search($values, &$context) {
  
  $context['message'] = t("Matching the Meta Tags");
}

/**
 * Batch 'finished' callback for Search Operation.
 *
 */
function metatag_bulk_uploader_finish_batch_search($success, $results, $operations) {
  if ($success) {
     }
  else {

    $error_operation = reset($operations);
    drupal_set_message(t("An error occurred while processing @operation with arguments : @args", array(
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE),
    )));
  }
}
