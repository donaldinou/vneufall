<?php
/**
 * @file immobilier-subscription-notification.tpl.php
 * Theme par défaut du message de notification de nouvelle(s) annonce(s)
 *
 * Available variables:
 * - $subscription
 * - $nodes
 *
 */
 ?>
 <html>
	<head>
		<h1><?php print count($nodes) ?> nouvelle(s) annonce(s) correspondent à votre recherche</h1>
		<?php foreach($nodes as $node) print theme("immobilier_subscription_notification_node", $node);?>	
		<em><a href="<?php print $subscription->unsubscribe ?>">Désinscription</a></em>
	</head>
</html>