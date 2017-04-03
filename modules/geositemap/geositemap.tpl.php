<h2>Principales villes de France</h2>
<table class="sitemap_table"><tr><td><ul>
<?foreach($villes as $i=>$ville):?>
	<li><?php print $ville->link?></li>
	<?if(($i+1)%ceil(count($villes)/3)==0):?></ul></td><td><ul><?endif?>
<?endforeach?>
</ul></td></tr></table>

<h2>Recherche par département</h2>
<table class="sitemap_table"><tr><td><ul>
	<?foreach($departements as $i=>$dpt):?>
		<li><?php print $dpt->link ?></li>
		<?if(($i+1)%ceil(count($departements)/3)==0):?></ul></td><td><ul><?endif?>
	<?endforeach?>
</ul></td></tr></table>