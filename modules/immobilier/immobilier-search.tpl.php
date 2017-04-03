<?php global $pager_page_array, $pager_total, $pager_total_items; ?>

<h2><?php print $pager_total_items[0] ?> annonces correspondent à votre recherche</h2>


<?php foreach($nodes as $key=>$node) { ?>
	<?php print theme("immobilier_search_node", $node); ?>
<? } ?>
<?php print theme('pager'); ?>