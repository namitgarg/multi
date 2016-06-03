*********************************************************
-----------------------MetaTag Bulk Uploader-------------------
*********************************************************

Introduction
------------
Metatag Bulk Uploader is used to Upload Metatag in bulk to your site.
It uses batch API to Upload Metatags from a CSV file.
Module supports 4 fields which are provided by Metatag module.

*This module provides a form where user will upload the CSV file.

*Csv file has 5 fields
  path -> you can enter node/nid or the URL Alias for the node.
  title -> this corresponds to Meta tag Title.
  description -> this corresponds to Meta tag description.
  abstract -> This corresponds to Meta abstract.
  keywords -> this correspOnds to Meta Keywords. 

* User can either upload all  field for Meta Tags or specific fields by leaving
  blank other fields
  
* User can either Url_alias or node/nid got the path cloumn. 

* A Sample CSV File can be found inside <sample> folder.

Requirements
------------

This module requires the following modules:

 * Metatag (https://www.drupal.org/project/metatag)
 * Chaos tool suite (ctools) (https://www.drupal.org/project/ctools)
 * Token (https://www.drupal.org/project/token)
 * Path (Core)
 * System (Core

Installation & Use
-------------------
* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

Configuration
-------------
 * Configure user permissions in Administration » People » Permissions:
   Metatag Bulk Uploader  - Select the roles you want to give permission to 
                            upload Metatags Using Metatag Bulk Uploader Module.

