*********************************************************
-----------------------MetaTag Import Export-------------------
*********************************************************

Introduction

------------
MetaTag Import Export can be used to import metatags form a CSV file or Export metatags to a CSV file for node pages.
Module supports 4 fields which are provided by Metatag module.


Description
-----------

  *This module has two seperate forms for import and export.
  *Export - you can select the content type and a CSV file will be downloaded to Your System.
  *Import - Upload the CSV file in the format given in Sample file and metatags will imported with success and error messages.
  *Csv file has 5 fields

  *  path -> you can enter node/nid or the URL Alias for the node.
  *  title -> this corresponds to Meta tag Title.
  *  description -> this corresponds to Meta tag description.
  *  abstract -> This corresponds to Meta abstract.
  *  keywords -> this correspOnds to Meta Keywords. 


  * User can either fill all 4 field for Meta Tags or specific fields by leaving blank other fields


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
 
Configure user permissions in Administration >> People >> Permissions:
    * Metatag Import  - Select the roles you want to give permission to import Metatags using the module.
    * Metatag Export  - Select the roles you want to give permission to export Metatags using the module.>
