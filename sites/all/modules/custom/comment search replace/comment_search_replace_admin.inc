<?php

function comment_search_replace_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = t("@typename", array('@typename' => $values1->name));
    $func_content_type[$type_machine_name] = $type_human_name;
  }
  return $func_content_type;
}

function comment_search_replace_form($form, &$form_state) {

  $content_type = comment_search_replace_type_list();
  $default_type = array('all' => 'All');
  $content_type = array_merge($default_type, $content_type);
  $case_list = array(
    "sensitive" => t("Sensitive"),
    "insensitive" => t("Insensitive"),
  );
  $session_match = FALSE;
  $form['search_box'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t("Search"),
    //'#default_value' => $intial_search,
    '#description' => t("Enter the text to be searched for"),
  );
  $form['replace_box'] = array(
    '#type' => 'textfield',
    '#title' => t("Replace"),
    //'#default_value' => $default_replace,
    '#description' => t("Enter the text to be replaced with"),
  );
  $form['content_type'] = array(
    '#type' => 'select',
    '#options' => $content_type,
    '#title' => t("Content type"),
    '#validated' => TRUE,
    '#required' => TRUE,
    //'#default_value' => $default_type,
    '#description' => t("Select the Content type"),
  );
  $form['case'] = array(
    '#type' => 'select',
    '#options' => $case_list,
    '#title' => t("Case Sensitive"),
    '#validated' => TRUE,
    '#required' => TRUE,
    // '#default_value' => $default_case,
    '#description' => t("Select whether to Search  Search with case Sensitive or not"),
  );
  $form['exact_match'] = array(
    '#type' => 'checkbox',
    '#options' => $session_match,
    '#title' => t("Exact Match"),
    '#validated' => TRUE,
    '#description' => t("Select to search exact words."),
      //'#default_value' => $session_match,
  );
  $form['search'] = array(
    '#type' => 'submit',
    '#value' => t("Search"),
    '#validate' => array('comment_search_replace_form_search_validate'),
    '#submit' => array('comment_search_replace_form_search_submit'),
  );
  $form['replace'] = array(
    '#type' => 'submit',
    '#value' => t("Replace"),
    '#validate' => array('comment_search_replace_form_replace_validate'),
    '#submit' => array('comment_search_replace_form_replace_submit'),
  );
  return $form;
}
/**
 * Form Validation for Search Operation.
 */
function comment_search_replace_form_search_validate($form, &$form_state) {
  
}

/**
 * Form Validation for Replace operation.
 */
function comment_search_replace_form_replace_validate($form, &$form_state) {
  
}

function comment_search_replace_form_search_submit($form, &$form_state) {
  $opn = 'search';
comment_search_replace_operation($form, $form_state, $opn);
}

function comment_search_replace_form_replace_submit($form, &$form_state) {
  $opn = 'replace';
  comment_search_replace_operation($form, $form_state, $opn);
}

function comment_search_replace_operation($form, &$form_state, $opn)
{
  $_SESSION['comment_search_replace_word_search'] = $form_state['values']['search_box'];
  $_SESSION['comment_search_replace_word_replace'] = $form_state['values']['replace_box'];
  $_SESSION['comment_search_replace_case'] = ($form_state['values']['case']);
  $_SESSION['comment_search_replace_match'] = ($form_state['values']['exact_match']);
  $_SESSION['comment_search_replace_show_result'] = "TRUE";
  $_SESSION['comment_search_replace_opn'] = $opn;
  $type = $form_state['values']['content_type'];
  $_SESSION['comment_search_replace_type'] = $type;
  
  $query = db_select('comment')
      ->fields('comment', array('cid','nid', 'subject'));
  if ($type != 'all') {
    $query->condition('type', $type);
  }
  
  print_r($_SESSION);
  die;
  
}