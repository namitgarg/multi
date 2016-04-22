<?php

function node_creator_form($form, &$form_state) {
  $content_type = node_creator_type_list();
  
  $field_info = node_creator_fields_name("page");
    print_r($field_info);
    print("fff");
    //$field_list[$field_info['machine_name']]=$field_info['human_name'];
  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#description' => t('Enter the text to be searched for'),
  );
  $form['content_type'] = array(
    '#type' => 'select',
    '#options' => $content_type,
    '#title' => t('Content type'),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#description' => t('Select the content type'),
  );
//  $form['field_name'] = array(
//    '#type' => 'select',
//    //'#options' => $field_list,
//    '#title' => t('Field Name'),
//    '#validated' => TRUE,
//    '#required' => TRUE,
//    '#description' => t('Select the Field Name'),
//  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    //'#validate' => array('metatag_replace_form_search_validate'),
    '#submit' => array('node_creator_submit'),
  );
  return $form;
}
/*
 * Function to get list of all content type
 */
function node_creator_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = $values1->name;
    $func_content_type[$type_machine_name] = $type_human_name;
  }

  return $func_content_type;
}

/*
 * Submit Handler for form
 */
function node_creator_submit($form, &$form_state) {
  print("<pre>");
  drupal_set_message("form submited ");
  node_creator_fieldsname($form_state['values']['content_type']);
  //node_creator_fetchdata("http://www.associatesinplasticsurgery.com");
}
/*
 * Function to fetch data from the web page
 */
function node_creator_fetchdata($url) {
    module_load_include('php', 'node_creator', 'include/simple_html_dom');  
  $html = file_get_html($url);
  foreach ($html->find('img') as $element) {
    print($element . '<br>') ;
  }
      die;
  drupal_set_message("gethtml");
}

function node_creator_fields_name($type)
{
  print($type);
  $field_list=field_info_instances('node',$type);
  $field_details=array();
  foreach($field_list as $values)
  {
    $field_details['human_name'][]=$values['label'];
    $field_details['machine_name'][]=$values['field_name'];
    $field_details['type'][]=$values['widget']['type'];
      
  }
  return $field_details;
  
}
