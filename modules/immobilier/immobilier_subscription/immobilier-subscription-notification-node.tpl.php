<?php
/**
 * @file immobilier-subscription-notification-node.tpl.php
 * Theme par défaut d'un node du message de notification de nouvelle(s) annonce(s)
 *
 * Available variables:
 * - $node
 *
 */
 ?>
 <fieldset>
	<img style="float:left" width="300" src="<?php print url($node->picture, array("absolute" => true)) ?>">
	<h2><?php print $node->title ?></h2>
	<p><?php print $node->body ?></p>
	<a href="<?php print $node->url ?>">Voir cette annonce</a>
</fieldset>