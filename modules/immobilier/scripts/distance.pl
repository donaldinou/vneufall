#!/usr/bin/perl
use strict;
my $pi = atan2(1,1) * 4;

my $coords1 = $ARGV[0];
my $coords2 = $ARGV[1];

my($lat1, $lon1) = split ",", $coords1;
my($lat2, $lon2) = split ",", $coords2;

print &distance($lat1,$lon1,$lat2,$lon2);

sub distance {
	my ($lat1, $lon1, $lat2, $lon2, $unit) = @_;
	my $theta = $lon1 - $lon2;
	my $dist = sin(deg2rad($lat1)) * sin(deg2rad($lat2)) + cos(deg2rad($lat1)) * cos(deg2rad($lat2)) * cos(deg2rad($theta));
	$dist  = acos($dist);
	$dist = rad2deg($dist);
	# miles
	$dist = $dist * 60 * 1.1515;
	# km
	$dist = $dist * 1.609344;
	# nautic miles
	#$dist = $dist * 0.8684;
	return ($dist);
}

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#:::  This function get the arccos function using arctan function   :::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
sub acos {
	my ($rad) = @_;
	my $ret = atan2(sqrt(1 - $rad**2), $rad);
	return $ret;
}

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#:::  This function converts decimal degrees to radians             :::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
sub deg2rad {
	my ($deg) = @_;
	return ($deg * $pi / 180);
}

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#:::  This function converts radians to decimal degrees             :::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
sub rad2deg {
	my ($rad) = @_;
	return ($rad * 180 / $pi);
}
#http://maps.google.fr/maps?saddr=48.111761000000001,-1.6802649999999999&daddr=48.109701999999999,-1.6793199999999999&dirflg=w