<?php

/*
 * @file include file for node creator form
 *  */

/*
 * Gets list of all content type
 */
function node_creator_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = t("@typename", array('@typename' => $values1->name));
    $func_content_type[$type_machine_name] = $type_human_name;
  }
  return $func_content_type;
}



/*
 * Main Form declaration 
 */

function node_creator_form($form, &$form_state) {
   $content_type = node_creator_type_list();
  $case_list = array(
    "sensitive" => t("Sensitive"),
    "insensitive" => t("Insensitive"),
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
      'callback' => 'node_creator_field_callback',
      'wrapper' => 'field-wrap',
    ),
  );
    
   $form['field_select'] = array(
    '#type' => 'select',
    //'#options' => $field_list,
     '#options' => node_creator_dependent_field($selected),
     '#multiple' => true,
    '#title' => t("Field"),
    '#validated' => TRUE,
    '#required' => TRUE,
    '#prefix' => '<div id="field-wrap">',
    '#suffix' => '</div>',
    //'#default_value' => $default_field,
    '#description' => t("Choose meta fields to be Searched"),
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

function node_creator_field_callback($form, $form_state)
{
    return $form['field_select'];

}

function node_creator_dependent_field($content_type)
{
  $field_list = field_info_instances('node',$content_type);
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