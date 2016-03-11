*********************************************************
-----------------------MetaTag Replace--------------------------------------
*********************************************************

Description
--------------------------------------------------------------------------------
This module provides a form for user to search and replace Meta tags.
There are options to select particular Content types .
Whether we want case sensitive search or not.
We can Match Exact Word or not.
User can choose to select to perform operation Individual Meta Fields or All
This Modules Creates Revision of each page it changes


Installation & Use
-------------------------------------------------------------------------------
1.  Enable module in module list located at administer > structure > modules.
2. Goto /admin/config/search/metatags/replace And Select the fields for Form
2. Configure user permissions in Administration > People > Permissions:

Limitations
--------------------------------------------------------------------------------

1. A user can only have one instance of Metatag Replace  running at a time.

2. Attempting to in two separate windows can lead to unknown errors.
   
Features
--------------------------------------------------------------------------------
1. Plain text search and replace.

2. Case sensitive search option.

3. Plain text search allows 'whole word' matching option.  For example, 
   searching for "catch" with the whole word option selected will filter out 
   "catching", but will match "catch".

4. Searches can be limited to specific content types.

5. Will save a new node revision when a replacement is made.

6. Search results for searches and replace can be Designed in Template file.