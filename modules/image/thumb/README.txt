/* $Id: README.txt,v 1.1 2008/11/24 09:28:45 davyvandenbremt Exp $ */

Description
-----------

With Thumb you can create thumbnails of your images. Thumb is under the hood
just an administration module around phpThumb
(http://phpthumb.sourceforge.net/).

Requirements
------------

This module requires Drupal 6.

Installation
------------

1) Copy/upload the thumb module folder to the sites/all/modules
directory of your Drupal installation.

2) Download phpThumb from http://phpthumb.sourceforge.net/#download.

3) Extract phpThumb in your module folder.

4) Enable the Thumb module in Drupal (administer -> modules).

5) Check if you don't see any Thumb errors on admin/reports/status.

Configuration
-------------

Define a preset for your thumbnail on administer -> build -> Thumb presets.

Usage
-----

Use thumb_url to generate the url for your thumbnail. Parameters are the
preset and the path to the original picture.

<?php
  $original_picture = 'http://farm4.static.flickr.com/3252/3021269164_e18fe3360a.jpg';
  $thumb = thumb_url('flickr_thumb', $original_picture);
?>

Alternative modules
-------------------

You could also use the excellent ImageCache module, which was built
specifically for Drupal. For generating thumbnails of local files this is a
much better solution. The only drawback is that it can't generate thumbnails
from remote files. That's the reason why Thumb was developed.

You can download ImageCache at
http://drupal.org/project/imagecache

Author
------

Davy Van Den Bremt <info@davyvandenbremt.be>
http://www.davyvandenbremt.be