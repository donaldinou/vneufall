#!/usr/bin/perl
BEGIN {
	use FindBin qw($Bin); 
	push(@INC, $Bin);
}
use Algorithm::GooglePolylineEncoding;

if($#ARGV <  2) {
	print "Usage : point_in_polygon.pl <encoded_polyline> <latitude> <longitude>\n";
	exit 1;
}

# args
($encoded_polyline, $latitude, $longitude) = @ARGV;
my @points = Algorithm::GooglePolylineEncoding::decode_polyline($encoded_polyline);

@polygon = ();
foreach $point (@points) {
    push(@polygon, $point->{lat});
    push(@polygon, $point->{lon});
}

my $return = point_in_polygon( $latitude, $longitude, @polygon );

if($return eq 1) {
	exit 0;
} else {
	exit 1;
}	

# point_in_polygon ( $x, $y, @xy )
#
#    Point ($x,$y), polygon ($x0, $y0, $x1, $y1, ...) in @xy.
#    Returns 1 for strictly interior points, 0 for strictly exterior
#    points. For the boundary points the situation is more complex and
#    beyond the scope of this book.  The boundary points are
#    exact, however: if a plane is divided into several polygons, any
#    given point belongs to exactly one polygon.
#
#    Derived from the comp.graphics.algorithms FAQ,
#    courtesy of Wm. Randolph Franklin.
#
sub point_in_polygon {
    my ( $x, $y, @xy ) = @_;

    my $n = @xy / 2;                      # Number of points in polygon.
    my @i = map { 2 * $_ } 0 .. (@xy/2);  # The even indices of @xy.
    my @x = map { $xy[ $_ ]     } @i;     # Even indices: x-coordinates.
    my @y = map { $xy[ $_ + 1 ] } @i;     # Odd indices: y-coordinates.

    my ( $i, $j );                        # Indices.

    my $side = 0;                         # 0 = outside, 1 = inside.

    for ( $i = 0, $j = $n - 1 ; $i < $n; $j = $i++ ) {
        if (
            (

             # If the y is between the (y-) borders ...
             ( ( $y[ $i ] <= $y ) && ( $y < $y[ $j ] ) ) ||
             ( ( $y[ $j ] <= $y ) && ( $y < $y[ $i ] ) )
            )
            and
            # ...the (x,y) to infinity line crosses the edge
            # from the ith point to the jth point...
            ($x
             <
             ( $x[ $j ] - $x[ $i ] ) *
             ( $y - $y[ $i ] ) / ( $y[ $j ] - $y[ $i ] ) + $x[ $i ] )) {
          $side = not $side; # Jump the fence.
      }
    }

    return $side ? 1 : 0;
}