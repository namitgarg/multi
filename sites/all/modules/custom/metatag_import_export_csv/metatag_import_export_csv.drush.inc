<?php
/**
 * 
 * @param type $command
 * @return type
 */
function metatag_import_export_csv_drush_help($command) {
  switch ($command) {
    case 'drush:metatag-import':
      return dt('This commans helps you to import metatag to node pages from a csv file
          The argument fot the command is filename
          follow the exampees'
          
          
          
          );
  }
}
drush_confirm('Are you sure you want to import metatags this will replace your old metatags ?', $indent = 0); 
/**
 * Implements hook_drush_command().
 */
function metatag_import_export_csv_drush_command() {
    $items['metatag-import'] = array(
      'description' => dt("Import metatags form a csv"),
      'arguments'   => array(
      'filepath'     => dt("The filepath to your file"),
         ),
      'examples' => array(
      'windows_example' => 'drush metatag-import D:\\drupal\\multi.csv',
    ),
    );
    return $items;
}

function drush_metatag_import_export_csv_metatag_import($filepath) {
 if(file_exists($filepath)) 
  {
      print($filepath);
      module_load_include('inc', 'metatag_import_export_csv', 'metatag_import_export_csv_upload_admin');
      metatag_import_export_csv_read_file($filepath);
}
else
{
drush_set_error(dt('File with the give path does not exists.'));
}
}





