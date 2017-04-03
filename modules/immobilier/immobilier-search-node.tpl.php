<h2><?php print $node->title ?></h2>
<?php print l(theme('image', $node->picture), $node->search->path."/detail", array("query" => array("page" => $node->search->index), "html" => true)) ?>
<p><?php print $node->body ?></p>
<?php print l("Voir le détail", $node->search->path."/detail", array("query" => array("page" => $node->search->index))) ?>
