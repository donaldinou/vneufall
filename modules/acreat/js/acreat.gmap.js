/* --------------------------------------------
* acreat.gmap.js
* copyright Acreat Web Technologies
*
* 
*  <script>var GOOGLE_MAP_API_KEY = "ABQIAAAAkcQmgo-y5ltSLbkj48jZ3RRPFqxWHkKw4NKsTTQWmieGta2m6RTdNyOJLhhZLYkRD9XOUg2k0Y3tdw";</script>
*  <script src="js/acreat.gmap.js" type="text/javascript"></script>
*
*/
if(typeof GOOGLE_MAP_API_KEY != "undefined")
	AcreatLoadGMapAPI(GOOGLE_MAP_API_KEY);


/* ---------------------------------------------------------------------------
* AcreatLoadGMap
*/
function AcreatLoadGMapAPI(key) {
	document.write('<script src="http://maps.google.com/maps?file=api&amp;v=2.x&amp;key=' + key + '" type="text/javascript"></' + 'script>');
}

/* ---------------------------------------------------------------------------
* AcreatGMap
*/
function AcreatGMap(id, coords, zoom) {
	if(!id) 	id = "gmap";
	if(!coords) coords = [48.109035906197036, -1.679534912109375];
	if(!zoom) 	zoom = 12;
	var gmap = false;
	if (GBrowserIsCompatible()) {
		var gmap = new GMap2(document.getElementById(id));
		gmap.addControl(new GSmallMapControl());
		gmap.addControl(new GMapTypeControl());
		gmap.setCenter(new GLatLng(coords[0], coords[1]), zoom);
		document.body.onunload = function() { GUnload(); }
	}
	return gmap;
}

/* ---------------------------------------------------------------------------
* AcreatGMapIcon
*/
function AcreatGMapIcon(url, width, height, shadow) {
	var newIcon = new GIcon(G_DEFAULT_ICON, url);
	newIcon.iconSize = new GSize(width, height);
	newIcon.iconAnchor	= new GPoint(width/2, height/2);
	newIcon.shadow = shadow;
	return newIcon;
}

/* ---------------------------------------------------------------------------
* AcreatGMapMarker
*/
function AcreatGMapMarker(glat,glng,icon,infoWindowHtml) {
	var marker=new GMarker(new GLatLng(glat,glng),{icon:icon, zIndexProcess:AcreatGMapMarker_zIndexOrder});
	if(infoWindowHtml)
		marker.bindInfoWindowHtml(infoWindowHtml);
	return marker;
}

/* ---------------------------------------------------------------------------
* AcreatGMapMarker_zIndexOrder
*/
function AcreatGMapMarker_zIndexOrder(marker,b) {
        return marker.zIndex ? marker.zIndex : GOverlay.getZIndex(marker.getPoint().lat());
}