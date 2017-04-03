/**
 * Module immobilier
 *
 * @see http://www.acreat.com
 * @depends Drupal 6
 * @author Acreat Web Technologies
 *
 */

/* ------------------------
* dpe_note
*/
function dpe_note(value) { 
	if(value <= 50) note = "A";
	else if(value > 50 && value <= 90) note = "B";
	else if(value > 90 && value <= 150) note = "C";
	else if(value > 150 && value <= 230) note = "D";
	else if(value > 230 && value <= 330) note = "E";
	else if(value > 330 && value <= 450) note = "F";
	else note = "G";
	return note;
	
}

/* ------------------------
* ges_note
*/
function ges_note(value) { 
	if(value <= 5) note = "A";
	else if(value > 5 && value <=10) note = "B";
	else if(value > 10 && value <= 20) note = "C";
	else if(value > 20 && value <= 35) note = "D";
	else if(value > 35 && value <= 55) note = "E";
	else if(value > 55 && value <= 80) note = "F";
	else note = "G";
	return note;
}

/* ------------------------
* localisation_gmap_init
*/
function localisation_gmap_init(lat, lon) { 
	gmap = new google.maps.Map2(document.getElementById("gmap"));
	gmap.addControl(new google.maps.SmallZoomControl3D());
	gmap.enableContinuousZoom();
	gmap.disableScrollWheelZoom();
	gmap.setCenter(new google.maps.LatLng(lat, lon), 15);
	google.maps.Event.addListener(gmap, "move", function() {
		var center = gmap.getCenter();
		$("#edit-latitude").val(center.lat());
		$("#edit-longitude").val(center.lng());
	});
	$("#edit-latitude,#edit-longitude").keyup(function() {
		gmap.setCenter(new google.maps.LatLng($("#edit-latitude").val(),$("#edit-longitude").val()));
	});
	gmap.set_default = function() {
		var search = $("#edit-adresse").val() + " " + $("#edit-code-postal").val() + " " + $("#edit-ville").val();
		var geocoder = new google.maps.ClientGeocoder();
		geocoder.getLatLng(search,function(point) {
			gmap.setCenter(point);
		});
	}
}
