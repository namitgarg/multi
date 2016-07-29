<?php
/**
 * 
 */


/**
 * 
 * @return type
 */
function comment_search_replace_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = t("@typename", array('@typename' => $values1->name));
    $func_content_type[$type_machine_name] = $type_human_name;
  }
  return $func_content_type;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function comment_search_replace_form($form, &$form_state) {

  $content_type = comment_search_replace_type_list();
  $case_list = array(
    "sensitive" => t("Sensitive"),
    "insensitive" => t("Insensitive"),
  );
  $role_options=array(
    'own' => t("Own Content"),   
  );
  if(user_access('comment_search_any'))
  {
    $any_role=array('any' => t('Any Content'));
    $role_options = array_merge($role_options,$any_role);
  }
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
  $selected = isset($form_state['values']['content_type']) ? $form_state['values']['content_type'] : key($content_type);
  $form['content_type'] = array(
    '#type' => 'select',
    '#options' => $content_type,
    '#title' => t("Content type"),
    '#validated' => TRUE,
    '#required' => TRUE,
    //'#default_value' => $default_type,
    '#description' => t("Select the Content type"),
        '#ajax' => array(
      'callback' => 'comment_search_replace_field_callback',
      'wrapper' => 'field-wrap',
    ),
  );
    
   $form['field_select'] = array(
    '#type' => 'select',
    //'#options' => $field_list,
     '#options' => comment_search_replace_dependent_field($selected),
    '#title' => t("Field"),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#prefix' => '<div id="field-wrap">',
    '#suffix' => '</div>',
    //'#default_value' => $default_field,
    '#description' => t("Choose meta fields to be Searched"),
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
   $form['role'] = array(
      '#type' => 'radios',
      '#options' => $role_options,
      '#title' => t("Search content For"),
      '#default_value' => 'own',
      '#description' => t("Search options."),
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
  $replace_word = $_SESSION['comment_search_replace_word_search'] =  $form_state['values']['search_box'];
  $replace_word = $_SESSION['comment_search_replace_word_replace'] =  $form_state['values']['replace_box'];
  $type = $_SESSION['comment_search_replace_type'] = $form_state['values']['content_type'];
  $field = $_SESSION['comment_search_replace_field'] = $form_state['values']['field_select']; 
  $case = $_SESSION['comment_search_replace_case'] = $form_state['values']['case'];
  $match = $_SESSION['comment_search_replace_match'] = $form_state['values']['exact_match'];
  $role = $_SESSION['comment_search_replace_role'] =  $form_state['values']['role'];
  $_SESSION['comment_search_replace_show_result'] = "TRUE";
  $_SESSION['comment_search_replace_opn'] = $opn;
 // Define The batch For Search Operation.
  $batch = array(
    'operations' => array(),
    'finished' => 'comment_search_replace_finish_batch_search',
    'title' => t("Processing  All Node For Searching"),
    'init_message' => t("Starting Comment Search Operation"),
    'progress_message' => t("Processed @current out of @total Nodes"),
    'file' => drupal_get_path('module', 'comment_search_replace') . '/comment_search_replace_admin.inc',
    'error_message' => t("Comment Search Raplce Module has encountered an error."),
  );  
  
  $query = db_select('comment','c');
  $query->join('node', 'n', 'c.nid = n.nid');
  $query->fields('c', array('cid'));
if($role == 'own')
  {
  global $user;
      $query->condition('c.uid', $user->uid);
  }
      $query->condition('n.type', $type);

    $results = $query->execute()->fetchAll();
print("<pre>");
foreach ($results as $values)
{
  $comment_cid_list[]=$values->cid;
       // $batch['operations'][] = array('comment_search_replace_start_batch_search', array($values->nid));

  $com=comment_load($values->cid);
}
    
print_r($comment_cid_list);
print_r($com);
die;  
}
function comment_search_replace_start_batch_search($values, &$context)
{
  
}
function comment_search_replace_finish_batch_search($success, $results, $operations)
{
  if ($success) {
    drupal_set_message("hiiii");
    print_r($results);
    
  }
    else {

    $error_operation = reset($operations);
    drupal_set_message(t("An error occurred while processing @operation with arguments : @args", array(
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE),
    )));
  }
  
  
}
function comment_search_replace_field_callback($form, $form_state)
{
    return $form['field_select'];

}
function comment_search_replace_dependent_field($content_type)
{
  $field_list = field_info_instances('comment', 'comment_node_' . $content_type);
  foreach ($field_list as $values) {
    if ($values['widget']['type'] == 'text_default' || 'text_textarea') {
      $field_list[$values['field_name']] = $values['label'];
    }
  }
  if(!empty($field_list))
  {
  $title_field = array('title' => 'Title');
  $field_list = array_merge($title_field, $field_list);
  $all_option=array('all'=>'All');
  $field_options=array_merge($field_list,$all_option);
  }
    return $field_options;
}