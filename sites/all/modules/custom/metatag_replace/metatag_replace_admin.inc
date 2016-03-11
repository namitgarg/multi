<?php

/**
 * @file
 * All the search and replace operatons are performed here.
 */

/**
 * Function to check for  Html and Php.
 *
 * @param array $data
 *   Field to be searched for.
 *
 * @return bool
 *   True if fields contains No Html or Php.
 */
function metatag_replace_xss_check($data) {
  $old_data = $data;
  $new_data = strip_tags($data);
  if ($old_data == $new_data) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function to get all Content Type details for Content type dropdown.
 */
function metatag_replace_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = $values1->name;
    $func_content_type[$type_machine_name] = $type_human_name;
  }

  return $func_content_type;
}

/**
 * Defines Main form for the module to Search and Replace.
 *
 * @param array $form
 *   Form API form.
 * @param array $form_state
 *   Form API form.
 *
 * @return array
 *   $form to return form.
 */
function metatag_replace_form($form, &$form_state) {
  drupal_session_start();
  $intial_search = '';
  $default_replace = '';
  $default_case = '';
  $default_field = '';
  $content_type = metatag_replace_type_list();
  // All Content type will be selected by deafault.
  $default_type = array('all' => 'All');
  // To Merge list of individual content type with all.
  $content_type = array_merge($default_type, $content_type);
  // List of Meta taga fields.
  $field_list = array(
    'title' => 'Title',
    'description' => 'Description',
    'abstract' => 'Abstract',
    'keywords' => 'Keywords',
    'all' => 'All Tags',
  );
  // To Select Case sensitivity.
  $case_list = array(
    'sensitive' => 'Sensitive',
    'insensitive' => 'Insensitive',
  );
  $session_match = FALSE;
  if ($_SESSION['show-result'] == 'TRUE') {
    $default_type = $_SESSION['stype'];
    $intial_search = $_SESSION['sword'];
    $default_case = $_SESSION['scase'];
    $session_match = $_SESSION['session_match'];
    $default_field = $_SESSION['field_result'];
    if ($session_match == 1) {
      $session_match = TRUE;
    }
    if (isset($_SESSION['rword'])) {
      $default_replace = $_SESSION['rword'];
    }
  }
  $form['search_box'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Search'),
    '#default_value' => $intial_search,
    '#description' => t('Enter The text to be searched for'),
  );
  $form['replace_box'] = array(
    '#type' => 'textfield',
    '#title' => t('Replace'),
    '#default_value' => $default_replace,
    '#description' => t('Enter The text to be Replaced With'),
  );
  $form['content_type'] = array(
    '#type' => 'select',
    '#options' => $content_type,
    '#title' => t('Select the Content Type'),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#default_value' => $default_type,
    '#description' => t('Select the content Type'),
  );
  $form['case'] = array(
    '#type' => 'select',
    '#options' => $case_list,
    '#title' => t('Select case sensitivity'),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#default_value' => $default_case,
    '#description' => t('Case sesitivity'),
  );
  $form['exact_match'] = array(
    '#type' => 'checkbox',
    '#options' => $session_match,
    '#title' => 'Select Exact Match',
    '#validated' => TRUE,
    '#description' => t('Select to search exact match.'),
    '#default_value' => $session_match,
  );
  $form['field_select'] = array(
    '#type' => 'select',
    '#options' => $field_list,
    '#title' => t('View results'),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#default_value' => $default_field,
    '#description' => t('Choose from title,Description or all'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Search',
    '#submit' => array('metatag_replace_form_submit_one'),
  );
  $form['submit1'] = array(
    '#type' => 'submit',
    '#value' => 'Replace',
    '#submit' => array('metatag_replace_form_submit_two'),
    '#validate' => array('metatag_replace_form_validate_two'),
  );

  /*
   * To Display the Result of The operation in the Field.
  */
   if ($_SESSION['show-result'] == 'TRUE') {
    if ($_SESSION['opn'] == 'complete') {
      $dispaly_result = $_SESSION['replace_result'];
    }
    elseif ($_SESSION['opn'] == 'replace' || $_SESSION['opn'] == 'search') {
      $dispaly_result = $_SESSION['search_result'];
    }
    $form['results'] = array(
      '#type' => 'markup',
      '#markup' => theme('pager'),
      '#theme' => 'metatag_replace',
      '#list' => array($dispaly_result),
    );
    $_SESSION['show-result'] = FALSE;
  }

  return $form;
}
/**
 * Form Validation for Search Operation.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 */
function metatag_replace_form_validate_one($form, &$form_state) {
  $output = metatag_replace_xss_check($form_state['values']['search_box']);
  if (!$output) {
    form_set_error('replace_box', t('Please Dont Enter any HTML or Php tags'));
  }
}
/**
 * Form Validation for Replace operation.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 */
function metatag_replace_form_validate_two($form, &$form_state) {
  if (empty($form_state['values']['replace_box'])) {
    form_set_error('replace_box', t('Replace Field is empty.'));
  }
  if ($form_state['values']['replace_box'] == $form_state['values']['search_box']) {
    form_set_error('replace_box', t('Search and Replace Texts are Same.'));
  }
  $output = metatag_replace_xss_check($form_state['values']['replace_box']);
  if ($output == FALSE) {
    form_set_error('replace_box', t('Please Dont Enter any HTML or Php tags.'));
  }
}

/**
 * Submmit Handler for Search Operation.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 */
function metatag_replace_form_submit_one($form, &$form_state) {
  $opn = 'search';
  metatag_replace_search_operation($form, $form_state, $opn);
}

/**
 * Submit Handler for Replace Operation.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 */
function metatag_replace_form_submit_two($form, &$form_state) {
  $opn = 'replace';
  metatag_replace_search_operation($form, $form_state, $opn);
}

/**
 * This function gets nid of nodes and pass to batch.
 *
 * $_SESSION['sword']=>Stores the Search word.
 * $_SESSION['rword']=> Stores The Replaced word.
 * $_SESSION['field_result'] => Stores field to be searched.
 * $_SESSION['scase']=> Stores the Case Sensitivity.
 * $_SESSION['session_match']=>Checks to search the exact word or not.
 * $_SESSION['opn']=> Stores Search, Replace according to Operation.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form APi form.
 * @param array $opn
 *   Stores which operation is performed.
 */
function metatag_replace_search_operation($form, &$form_state, $opn) {
  $_SESSION['sword'] = $form_state['values']['search_box'];
  $_SESSION['rword'] = $form_state['values']['replace_box'];
  $_SESSION['field_result'] = $form_state['values']['field_select'];
  $_SESSION['scase'] = $form_state['values']['case'];
  $_SESSION['session_match'] = $form_state['values']['exact_match'];
  $_SESSION['show-result'] = "TRUE";
  $_SESSION['opn'] = $opn;
  $type = $form_state['values']['content_type'];
  $_SESSION['stype'] = $type;
  // Define The batch For Search Operation.
  $batch = array(
    'operations' => array(),
    'finished' => 'metatag_replace_finish_batch_search',
    'title' => t('Processing  All Node For Searching'),
    'init_message' => t('Starting Metatag Search Operation'),
    'progress_message' => t('Processed @current out of @total Nodes'),
    'file' => drupal_get_path('module', 'metatag_replace') . '/metatag_replace_admin.inc',
    'error_message' => t('MetaTag Module has encountered an error.'),
  );
  // Query to get nid from node table.
  $query = db_select('node', 'vd')
      ->fields('vd', array('nid', 'title'));
  if ($type != 'all') {
    $query->condition('type', $type);
  }
  elseif ($type = 'all') {
    $type = metatag_replace_type_list();
    $type = array_keys($type);
    $query->condition('type', ($type), 'IN');
  }
  $results = $query->execute()->fetchAll();
  // Call The Batch Process.
  // $values Stores the nid Of all Nodes of Selected Type.
  foreach ($results as $values) {
    $batch['operations'][] = array('metatag_replace_start_batch_search', array($values->nid));
  }
  batch_set($batch);
  // The path to redirect when batch is done.
  batch_process('admin/config/search/metatags/replace');
}

/**
 * Batch Operation Callback.
 *
 * $context['results']['matches'] =>Stores the result of Search operation.
 * $context['results']['nid']=>Stores the Nid of The found nodes.
 * $context['results']['node_title']=>Stores The Node title.
 * $context['results']['matches1']=>Stores the Second field result.
 * $context['results']['nid1']=>Stores the nid for Second result.
 * $context['results']['node_title1']=>Stores the node Title for Second Field.
 * 
 * @param array $values
 *   Stores the nid of nodes.
 * @param array $context
 *   The $context array gathers batch context information about the execution.
 */
function metatag_replace_start_batch_search($values, &$context) {
  $nid_list = $values;
  $searchword = $_SESSION['sword'];
  $replaceword = $_SESSION['rword'];
  $context['results']['search-word'] = $searchword;
  $context['results']['replace-word'] = $replaceword;
  $context['results']['opn'] = $_SESSION['opn'];
  $node = node_load($nid_list);
  // To check all nodes that have Meta tags and store them.
  if (!empty($node->metatags[$node->language]['title']) || !empty($node->metatags[$node->language]['description']) || !empty($node->metatags[$node->language]['abstract']) || !empty($node->metatags[$node->language]['keywords'])) {
    if ($_SESSION['field_result'] == 'title') {
      $meta_title = $node->metatags[$node->language]['title']['value'];
      $context['message'] = "Meta Title is working...";
      $context['results']['field_result'] = "title";
      $match_field = $meta_title;
    }
    elseif ($_SESSION['field_result'] == 'description') {
      $meta_description = $node->metatags[$node->language]['description']['value'];
      $context['message'] = "Meta Description is working...";
      $context['results']['field_result'] = "description";
      $match_field = $meta_description;
    }
    elseif ($_SESSION['field_result'] == 'abstract') {
      $meta_abstract = $node->metatags[$node->language]['abstract']['value'];
      $context['message'] = "Meta Abstract is working...";
      $context['results']['field_result'] = "abstract";
      $match_field = $meta_abstract;
    }
    elseif ($_SESSION['field_result'] == 'keywords') {
      $meta_keywords = $node->metatags[$node->language]['keywords']['value'];
      $context['message'] = "Meta Keywords is working...";
      $context['results']['field_result'] = "keywords";
      $match_field = $meta_keywords;
    }
    elseif ($_SESSION['field_result'] == 'all') {
      $meta_title = $node->metatags[$node->language]['title']['value'];
      $meta_description = $node->metatags[$node->language]['description']['value'];
      $meta_abstract = $node->metatags[$node->language]['abstract']['value'];
      $meta_keywords = $node->metatags[$node->language]['keywords']['value'];
      $context['message'] = "Meta All Tags  is working ....";
      $context['results']['field_result'] = "all";
    }
    $node_title = $node->title;
    $meta_nid = $values;
  }
  // To match individual Meta Fields.
  if ($_SESSION['field_result'] != 'all') {
    if ($_SESSION['session_match'] == '1') {
      if ($_SESSION['scase'] == 'insensitive') {
        preg_match("/\b$searchword\b/i", $match_field, $matches);
        if (!empty($matches)) {
          $context['results']['matches'][] = $match_field;
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
      else {
        preg_match("/\b$searchword\b/", $match_field, $matches);
        if (!empty($matches)) {
          $context['results']['matches'][] = $match_field;
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
    }
    else {
      if ($_SESSION['scase'] == 'insensitive') {
        preg_match("/$searchword/i", $match_field, $matches);
        if (!empty($matches)) {
          $context['results']['matches'][] = $match_field;
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
      else {

        preg_match("/$searchword/", $match_field, $matches);
        if (!empty($matches)) {
          $context['results']['matches'][] = $match_field;
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
    }
  }
  // To Match All Meta Tag Fields.
  elseif ($_SESSION['field_result'] == 'all') {
    if ($_SESSION['session_match'] == '1') {
      if ($_SESSION['scase'] == 'insensitive') {
        preg_match("/\b$searchword\b/i", $meta_title, $matches);
        preg_match("/\b$searchword\b/i", $meta_description, $matches1);
        preg_match("/\b$searchword\b/i", $meta_abstract, $matches2);
        preg_match("/\b$searchword\b/i", $meta_keywords, $matches3);
        if (!empty($matches) || !empty($matches1) || !empty($matches2) || !empty($matches3)) {
          if (!empty($matches)) {
            $context['results']['matches']['title'][] = $meta_title;
          }
          else {
            $context['results']['matches']['title'][] = "";
          }
          if (!empty($matches1)) {
            $context['results']['matches']['description'][] = $meta_description;
          }
          else {
            $context['results']['matches']['description'][] = "";
          }
          if (!empty($matches2)) {
            $context['results']['matches']['abstract'][] = $meta_abstract;
          }
          else {
            $context['results']['matches']['abstract'][] = "";
          }
          if (!empty($matches3)) {
            $context['results']['matches']['keywords'][] = $meta_keywords;
          }
          else {
            $context['results']['matches']['keywords'][] = "";
          }
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
      else {
        preg_match("/\b$searchword\b/i", $meta_title, $matches);
        preg_match("/\b$searchword\b/i", $meta_description, $matches1);
        preg_match("/\b$searchword\b/i", $meta_abstract, $matches2);
        preg_match("/\b$searchword\b/i", $meta_keywords, $matches3);
        if (!empty($matches) || !empty($matches1) || !empty($matches2) || !empty($matches3)) {
          if (!empty($matches)) {
            $context['results']['matches']['title'][] = $meta_title;
          }
          else {
            $context['results']['matches']['title'][] = "";
          }
          if (!empty($matches1)) {
            $context['results']['matches']['description'][] = $meta_description;
          }
          else {
            $context['results']['matches']['description'][] = "";
          }
          if (!empty($matches2)) {
            $context['results']['matches']['abstract'][] = $meta_abstract;
          }
          else {
            $context['results']['matches']['abstract'][] = "";
          }
          if (!empty($matches3)) {
            $context['results']['matches']['keywords'][] = $meta_keywords;
          }
          else {
            $context['results']['matches']['keywords'][] = "";
          }
          $context['results']['nid_all'][] = $meta_nid;
          $context['results']['node_title_all'][] = $node_title;
        }
      }
    }
    else {

      if ($_SESSION['scase'] == 'insensitive') {
        preg_match("/$searchword/i", $meta_title, $matches);
        preg_match("/$searchword/i", $meta_description, $matches1);
        preg_match("/$searchword/i", $meta_abstract, $matches2);
        preg_match("/$searchword/i", $meta_keywords, $matches3);
        if (!empty($matches) || !empty($matches1) || !empty($matches2) || !empty($matches3)) {
          if (!empty($matches)) {
            $context['results']['matches']['title'][] = $meta_title;
          }
          else {
            $context['results']['matches']['title'][] = "";
          }
          if (!empty($matches1)) {
            $context['results']['matches']['description'][] = $meta_description;
          }
          else {
            $context['results']['matches']['description'][] = "";
          }
          if (!empty($matches2)) {
            $context['results']['matches']['abstract'][] = $meta_abstract;
          }
          else {
            $context['results']['matches']['abstract'][] = "";
          }
          if (!empty($matches3)) {
            $context['results']['matches']['keywords'][] = $meta_keywords;
          }
          else {
            $context['results']['matches']['keywords'][] = "";
          }
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
      else {
        preg_match("/$searchword/", $meta_title, $matches);
        preg_match("/$searchword/", $meta_description, $matches1);
        preg_match("/$searchword/", $meta_abstract, $matches2);
        preg_match("/$searchword/", $meta_keywords, $matches3);
        if (!empty($matches) || !empty($matches1) || !empty($matches2) || !empty($matches3)) {
          if (!empty($matches)) {
            $context['results']['matches']['title'][] = $meta_title;
          }
          else {
            $context['results']['matches']['title'][] = "";
          }
          if (!empty($matches1)) {
            $context['results']['matches']['description'][] = $meta_description;
          }
          else {
            $context['results']['matches']['description'][] = "";
          }

          if (!empty($matches2)) {
            $context['results']['matches']['abstract'][] = $meta_abstract;
          }
          else {
            $context['results']['matches']['abstract'][] = "";
          }
          if (!empty($matches3)) {
            $context['results']['matches']['keywords'][] = $meta_keywords;
          }
          else {
            $context['results']['matches']['keywords'][] = "";
          }
          $context['results']['nid'][] = $meta_nid;
          $context['results']['node_title'][] = $node_title;
        }
      }
    }
  }
  $context['message'] = "Matching the Meta Tags";
}

/**
 * Batch 'finished' callback.
 *
 * @param array $success
 *   The success parameter means no fatal PHP errors were detected.
 * @param array $results
 *   Stores the result of Batch process.
 * @param array $operations
 *   Stores Whether it is Search or Replace operation.
 */
function metatag_replace_finish_batch_search($success, $results, $operations) {
  if ($success) {
    $_SESSION['search_result'] = $results;
    $opn = $results['opn'];
    $searchword = $results['search-word'];
    $replaceword = $results['replace-word'];
    $count = 0;
    if (!empty($results['nid'])) {
      $count = count($results['nid']);
    }
    $flag = TRUE;
    if ($count == 0) {
      drupal_set_message(t("No Matches Found"));
      $form_state['redirect'] = 'admin/config/search/metatags/replace';
      $flag = FALSE;
    }
    if ($opn == 'search' && $flag) {
      $_SESSION['show-result'] = 'TRUE';
      drupal_set_message(t("We found occurrences in %count Pages", array('%count' => $count)), 'status');
    }
    elseif ($opn == 'replace' && $flag) {
      $_SESSION['show-result'] = 'TRUE';
      drupal_set_message(t("Do You want to replace [%searchword] with [%replaceword] at %count pages", array(
        '%searchword' => $searchword,
        '%replaceword' => $replaceword,
        '%count' => $count,
          )));
      drupal_goto('admin/config/search/metatags/replace/confirm');
    }
  }
  else {

    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array(
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE)))
        );
  }
}

/**
 * Confirm Form Function.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 * 
 * @return form
 *   Return the array.
 */
function metatag_replace_confirm_form($form, &$form_state) {
  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
    '#submit' => array('metatag_replace_confirm_form_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('metatag_replace_cancel_form_submit'),
  );
  if ($_SESSION['show-result'] == 'TRUE') {

    $form['results'] = array(
      '#type' => 'markup',
      '#theme' => 'metatag_replace',
      '#list' => array($_SESSION['search_result']),
    );
  }
  return $form;
}

/**
 * Implements Submit Handler to Confirm Replace.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 */
function metatag_replace_confirm_form_submit($form, &$form_state) {
  $_SESSION['opn'] = 'complete';
  $batch1 = array(
    'operations' => array(),
    'finished' => 'metatag_replace_batch_fix_finished',
    'title' => t('Replacing Meta tags Fields'),
    'init_message' => t('Metatag Replace Operation'),
    'progress_message' => t('Processed @current out of @total Nodes'),
    'file' => drupal_get_path('module', 'metatag_replace') . '/metatag_replace_admin.inc',
    'error_message' => t('Meta tag Module has encountered an error.'),
  );
  if ($_SESSION['search_result']['field_result'] != 'all') {
    $replace_list = $_SESSION['search_result']['matches'];
    $nid_replace = $_SESSION['search_result']['nid'];
    $node_replace_title = $_SESSION['search_result']['node_title'];
    foreach ($node_replace_title as $keys => $values) {
      $batch1['operations'][] = array('metatag_replace_start_batch_replace_single', array(
          $values,
          $nid_replace[$keys],
          $replace_list[$keys],
        ),
      );
    }
  }
  else {
    $node_replace_title = $_SESSION['search_result']['node_title'];
    $nid_replace = $_SESSION['search_result']['nid'];
    $replace_list_title = $_SESSION['search_result']['matches']['title'];
    $replace_list_description = $_SESSION['search_result']['matches']['description'];
    $replace_list_abstract = $_SESSION['search_result']['matches']['abstract'];
    $replace_list_keywords = $_SESSION['search_result']['matches']['keywords'];
    foreach ($node_replace_title as $keys => $values) {
      $batch1['operations'][] = array('metatag_replace_start_batch_replace_all', array(
          $values,
          $nid_replace[$keys],
          $replace_list_title[$keys],
          $replace_list_description[$keys],
          $replace_list_abstract[$keys],
          $replace_list_keywords[$keys],
        ),
      );
    }
  }
  batch_set($batch1);
  batch_process('admin/config/search/metatags/replace/');
}

/**
 * Batch Process for Single Field.
 *
 * @param array $node_title
 *   Title of node.
 * @param array $nid
 *   Nid of the Node.
 * @param array $meta_list
 *   Meta Field for The node.
 * @param array $context
 *   Batch array to Store Variables.
 */
function metatag_replace_start_batch_replace_single($node_title, $nid, $meta_list, &$context) {
  $searchword = $_SESSION['sword'];
  $replaceword = $_SESSION['rword'];
  if ($_SESSION['session_match'] == '1') {
    if ($_SESSION['scase'] == 'insensitive') {
      $replace_result = preg_replace("/\b$searchword\b/i", $replaceword, $meta_list);
    }
    else {
      $replace_result = preg_replace("/\b$searchword\b/", $replaceword, $meta_list);
    }
  }
  else {
    if ($_SESSION['scase'] == 'insensitive') {
      $replace_result = preg_replace("/$searchword/i", $replaceword, $meta_list);
    }
    else {
      $replace_result = preg_replace("/$searchword/", $replaceword, $meta_list);
    }
  }
  $context['results']['replaced'][] = $replace_result;
  $context['results']['nid'][] = $nid;
  $context['results']['node_title'][] = $node_title;
  $context['results']['opn'] = "complete";
  $context['results']['field_result'] = $_SESSION['field_result'];
  $node = node_load($nid);
  $node->metatags[$node->language][$_SESSION['field_result']]['value'] = $replace_result;
  $node->revision = 1;
  $node->log = "Modified by Meta tag replace module Replaced $searchword with $replaceword";
  node_save($node);
}

/**
 * This is the batch process for Replace operation.
 *
 * @param array $node_title
 *   Stores the title of node.
 * @param array $nid
 *   Stores the nid of node.
 * @param array $meta_list
 *   Stores the first result of operation.
 * @param array $meta_description
 *   Stores the second result of operation.
 * @param array $context
 *   Contains information about the batch.
 */
function metatag_replace_start_batch_replace_all($node_title, $nid, $meta_list, $meta_description, $meta_abstract, $meta_keywords, &$context) {
  $searchword = $_SESSION['sword'];
  $replaceword = $_SESSION['rword'];
  if ($_SESSION['session_match'] == '1') {
    if ($_SESSION['scase'] == 'insensitive') {
      $replace_result = preg_replace("/\b$searchword\b/i", $replaceword, $meta_list);
      $replace_result2 = preg_replace("/\b$searchword\b/i", $replaceword, $meta_description);
      $replace_result3 = preg_replace("/\b$searchword\b/i", $replaceword, $meta_abstract);
      $replace_result4 = preg_replace("/\b$searchword\b/i", $replaceword, $meta_keywords);
    }
    else {
      $replace_result = preg_replace("/\b$searchword\b/", $replaceword, $meta_list);
      $replace_result2 = preg_replace("/\b$searchword\b/", $replaceword, $meta_description);
      $replace_result3 = preg_replace("/\b$searchword\b/", $replaceword, $meta_abstract);
      $replace_result4 = preg_replace("/\b$searchword\b/", $replaceword, $meta_keywords);
    }
  }
  else {
    if ($_SESSION['scase'] == 'insensitive') {
      $replace_result = preg_replace("/$searchword/i", $replaceword, $meta_list);
      $replace_result2 = preg_replace("/$searchword/i", $replaceword, $meta_description);
      $replace_result3 = preg_replace("/$searchword/i", $replaceword, $meta_abstract);
      $replace_result4 = preg_replace("/$searchword/i", $replaceword, $meta_keywords);
    }
    else {
      $replace_result = preg_replace("/$searchword/", $replaceword, $meta_list);
      $replace_result2 = preg_replace("/$searchword/", $replaceword, $meta_description);
      $replace_result3 = preg_replace("/$searchword/", $replaceword, $meta_abstract);
      $replace_result4 = preg_replace("/$searchword/", $replaceword, $meta_keywords);
    }
  }
  $context['results']['replaced']['title'][] = $replace_result;
  $context['results']['replaced']['description'][] = $replace_result2;
  $context['results']['replaced']['abstract'][] = $replace_result3;
  $context['results']['replaced']['keywords'][] = $replace_result4;
  $context['results']['nid'][] = $nid;
  $context['results']['node_title'][] = $node_title;
  $context['results']['opn'] = "complete";
  $context['results']['field_result'] = $_SESSION['field_result'];
  $node = node_load($nid);
  if ($replace_result != "") {
    $node->metatags[$node->language]['title']['value'] = $replace_result;
  }
  if ($replace_result2 != "") {
    $node->metatags[$node->language]['description']['value'] = $replace_result2;
  }
  if ($replace_result3 != "") {
    $node->metatags[$node->language]['abstract']['value'] = $replace_result3;
  }
  if ($replace_result4 != "") {
    $node->metatags[$node->language]['keywords']['value'] = $replace_result4;
  }
  $node->revision = 1;
  $node->log = "Modified by Meta tag replace module Replaced $searchword with $replaceword";
  node_save($node);
}

/**
 * This Function is Called when replace operation Ends.
 *
 * @param array $success
 *   The success parameter means no fatal PHP errors were detected.
 * @param array $results
 *   Stores the result of Batch process.
 * @param array $operations
 *   Stores Whether it is Search or Replace operation.
 */
function metatag_replace_batch_fix_finished($success, $results, $operations) {
  if ($success) {
    $_SESSION['replace_result'] = $results;
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array(
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE)))
        );
  }
}

/**
 * Submit Handler for Cancel Operation for Replace Operation.
 *
 * @param array $form
 *   Form Api form.
 * @param array $form_state
 *   Form Api form.
 */
function metatag_replace_cancel_form_submit($form, &$form_state) {
  drupal_set_message(t("You pressed Cancel"));
  $form_state['redirect'] = 'admin/config/search/metatags/replace';
}