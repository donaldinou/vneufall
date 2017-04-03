#!/usr/bin/perl
use strict;

if ($#ARGV == -1) {
	print "usage: geocoding.pl <adress> (<bounds>)\n";
	exit
}

my $str = $ARGV[0];
my $bounds = $ARGV[1];
$str = &escape($str);

my $url = "http://maps.google.com/maps/api/geocode/xml?address=$str&sensor=false&region=fr";
if($bounds) { 
	#$url .= "&bounds=".escape($bounds); 
}

my $xml = `GET '$url'`;
my ($dpt) = $xml =~ m!>(\w{2})<\/short_name>\s+<type>administrative_area_level_2!sig;
if ($xml =~ m!<result>\s+<type>([^<]*).*<location>\s+<lat>([^<]*)<\/lat>\s+<lng>([^<]*)!sig) {
	print "$dpt,$1,$2,$3";
}

sub escape {
	my($toencode) = @_;
	$toencode=~s/([^a-zA-Z0-9_\-. ])/uc sprintf("%%%02x",ord($1))/eg;
	$toencode =~ tr/ /+/; # spaces become pluses
	return $toencode;
}

#http://maps.google.com/maps/api/geocode/xml?address=La%20Plessette,%2035230%20ORGERES,%20FRANCE&sensor=false&region=FR&bounds=47.84265762816535,-2.548828125|48.79239019646406,-1.0986328125