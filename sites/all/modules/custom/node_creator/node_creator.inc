<?php

function node_creator_type_list() {
  $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name = $values1->type;
    $type_human_name = $values1->name;
    $func_content_type[$type_machine_name] = $type_human_name;
  }

  return $func_content_type;
}



function node_creator_form($form, &$form_state) {
  $content_type = node_creator_type_list();
  $my_fields = field_info_instances("node", "pages");
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
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    //'#validate' => array('metatag_replace_form_search_validate'),
    '#submit' => array('node_creator_submit'),
  );
  return $form;
}

function node_creator_submit($form, &$form_state) {
  drupal_set_message("form submited ");
  node_creator_fetchdata("http://www.associatesinplasticsurgery.com");
}
function node_creator_fetchdata($url) {
    module_load_include('php', 'node_creator', 'include/simple_html_dom');  
  $html = file_get_html($url);
//foreach($html->find('span') as $element)
//{
       //echo $element.'<br>';
//}
  foreach ($html->find('img') as $element) {
    print($element . '<br>') ;

  }
      die;
  drupal_set_message("gethtml");
}
