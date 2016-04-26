<?php

/*
 * @file include file for node creator form
 *  */

/*
 * Gets list of all content type
 */

function node_creator_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values) {
    $type_machine_name = $values->type;
    $type_human_name = $values->name;
    $func_content_type[$type_machine_name] = $type_human_name;
  }
  return $func_content_type;
}

/**
 * Gets list of fields in a content type
 */
function node_creator_fields_name($type) {
  drupal_set_message($type);
  $field_list = field_info_instances('node', $type);
  $field_details = array();
  foreach ($field_list as $values) {
    if(isset($values['label'])){
      $field_details[$values['field_name']] = $values['label'];
    $field_type[] = $values['widget']['type'];
    }
    
  }
  return $field_details;
}

/*
 * Main Form declaration 
 */

function node_creator_form($form, &$form_state) {
  $content_type = node_creator_type_list();
  $field_info = node_creator_fields_name("article");
    $selected = isset($form_state['values']['content_type']) ? $form_state['values']['content_type'] :array('body'=>'Body');

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
    '#ajax' => array(
      'callback' => 'node_creator_fields_dropdown_callback',
      'wrapper' => 'dropdown-second-replace',
    ),
  );

  $form['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Field Name'),
    '#prefix' => '<div id="dropdown-second-replace">',
    '#suffix' => '</div>',
    '#validated' => TRUE,
    '#required' => TRUE,
    '#options' => node_creator_fields_name($form_state['values']['content_type']),
    '#default' => array('body'=>'Body'),
    '#description' => t('Select the Field Name'),
  );
  $form['upload_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload File'),
    '#required' => True,
    '#description' => t('Allowed Extensions: csv'),
    '#upload_location' => 'public://node_creator/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('csv'),
      'file_validate_size' => array(1 * 1024 * 1024),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#submit' => array('node_creator_submit'),
  );
  return $form;
}

function node_creator_fields_dropdown_callback($form, &$form_state)
{
  return $form['field_name'];
}
/*
 * Submit Handler for form
 */

function node_creator_submit($form, &$form_state) {
  print("<pre>");
  drupal_set_message("form submited ");
  $file = file_load($form_state['values']['upload_file']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  $fileurl = file_create_url($file->uri);

  $row = 1;
  if (($handle = fopen($fileurl, "r")) !== FALSE) {
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
      $num = count($data);
      $row++;
      for ($c = 0; $c < $num; $c++) {
        echo $data[$c] . "<br />\n";
      }
    }
    fclose($handle);
  }
  //node_creator_fetchdata("http://www.associatesinplasticsurgery.com");
}

/*
 * Function to fetch data from the web page
 */

function node_creator_fetchdata($url) {
  module_load_include('php', 'node_creator', 'includes/simple_html_dom');
  $html = file_get_html($url);
  foreach ($html->find('img') as $element) {
    print($element . '<br>');
  }
  die;
  drupal_set_message("gethtml");
}
