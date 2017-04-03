<h2>Notre activité n°1 #a,ville#</h2>
<?php //print geositemap_random_text("welcome",6,2) ?>
<h2>Notre activité n°2 #a,ville#</h2>
<?php //print geositemap_random_text("activity2") ?>

<?php if(!empty($arrondissements)) { ?>
	<h2>Arrondissements #de,ville#</h2>
	<ul>
		<? foreach($arrondissements as $arrondissement) { ?>
		<li><?php print $arrondissement->link ?></li>
		
		<? } ?>
	</ul>

<? } ?>

<h2>Voir aussi #a,ville#</h2>
<ul class="voir_aussi">
	<li>
		<img src="http://www.viteloge.com/img/viteloge_logo_120.png" alt="Annonces immobilière #a,ville#">
		<a href="http://www.viteloge.com/sitemap/immobilier_<?=strtourl($ville->article_nom)?>_<?=$ville->codeInsee?>.html"><strong>Immobilier #ville#</strong></a> : 
	grâce à Vitelogé, le <strong>moteur de recherche immobilier le plus rapide</strong>, retrouvez toutes les offres immobilières #de,ville# et ses alentours 
	(<a href="http://www.viteloge.com/sitemap/immobilier_vente_appartement_<?=strtourl($ville->article_nom)?>_<?=$ville->codeInsee?>.html">vente appartement #a,ville#</a>, 
	<a href="http://www.viteloge.com/sitemap/immobilier_vente_maison_<?=strtourl($ville->article_nom)?>_<?=$ville->codeInsee?>.html">vente maison #a,ville#</a>) 
	indexés sur les sites immobiliers des professionnels de l'immobilier #de,ville#.
	</li>
</ul>