<?php
function node_creator_form($form, &$form_state)
{
 
  $form['search_box'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Search'),
    '#description' => t('Enter the text to be searched for'),
  );
  return $form;
}