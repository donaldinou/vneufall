<? print geositemap_random_text("departement",6,1) ?>

<h2>Principales villes #de,departement#</h2>
<table class="sitemap_table"><tr><td><ul>
<?foreach($top_villes as $i=>$ville):?>
	<li><?php print $ville->link?></li>
	<?if(($i+1)%ceil(count($top_villes)/3)==0):?></ul></td><td><ul><?endif?>
<?endforeach?>
</ul></td></tr></table>

<h2>Toutes les villes #de,departement#</h2>
<ul class="letters">
	<? $i = 0; foreach($lettres as $lettre) { ?>
		<li><? print l($lettre, $i ? $departement->link_path . "/index/".strtolower($lettre) : $departement->link_path); $i++ ?></li>
	<? } ?>
</ul>

<table class="sitemap_table"><tr><td><ul>
	<?foreach($villes as $i=>$ville):?>
		<li><?php print $ville->link ?></li>
		<?if(($i+1)%ceil(count($villes)/3)==0):?></ul></td><td><ul><?endif?>
	<?endforeach?>
</ul></td></tr></table>