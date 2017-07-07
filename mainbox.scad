use <roundedsquare.scad>;
use <tubemesh.scad>;

megaWidth = 53.34;
megaLength = 101.6;
megaPCBThickness = 1.66;
megaHoles = [ [  50.8, 13.97 ],
		[  50.8, 96.52 ]];
/* [[x,z,width,height],...] */
megaFrontCutouts = [ [7, megaPCBThickness, 12.23, 10.82],
    [ megaWidth-9.04-6.7, megaPCBThickness, 9.04, 11.08 ] ];
megaBackCutouts = [];
cbWidth = 54.9;
cbLength = 99.06; // between edges of RJ-45 breakout board
cbPCBThickness = 1.7;
cbFrontCutouts = [[5.88,5.45,15.15,11.56],[cbWidth-5.58-15.15,5.45,15.15,11.56]];
cbBackCutouts = [[cbWidth-5.58-15.15,5.45,15.15,11.56]];
cbHoles = [[4.04,cbLength-7.5]];

boardDivider = 3;
underPCBs = 5;

fitTolerance = 0.4;
cutoutTolerance = 1;
bottomSlideWidth = 1.82;
topSlideWidth = 1.75;

sideWallThickness = 1.25;
bottomThickness = 1.5;
topThickness = 1.5;

corner = 7;

module dummy() {}

nudge = 0.001;
insideWidth = boardDivider+4*fitTolerance+megaWidth+cbWidth;
insideLength = max(megaLength,cbLength);
insideLengthN = insideLength+2*nudge;

cbX = fitTolerance;
megaX = 3*fitTolerance+cbWidth+boardDivider;

module boards() {
    color("blue") {
        translate( [megaX,fitTolerance,underPCBs] ) cube([megaWidth, megaLength, megaPCBThickness]);
    }
    color("red") {
        translate( [cbX,fitTolerance,underPCBs])
        cube([cbWidth, cbLength, cbPCBThickness]);
    }
}

module boardMounts() {
    module diagonalSlideLeft() {
        leftPointsAndFaces = 
            pointsAndFaces([ [ [0,0,0], [topSlideWidth,0,topSlideWidth], [0,0,topSlideWidth] ], 
              [ [0,insideLengthN,0], [topSlideWidth,insideLengthN,topSlideWidth], [0,insideLengthN,topSlideWidth] ] ]);
        polyhedron(points=leftPointsAndFaces[0], faces=leftPointsAndFaces[1]);
    }
    
    module diagonalSlideRight() {
        mirror([1,0,0]) diagonalSlideLeft();
    }
    
    cube([bottomSlideWidth+nudge, insideLengthN, underPCBs]);
    translate([2*fitTolerance+cbWidth-bottomSlideWidth,0,0]) 
        cube([bottomSlideWidth*2+boardDivider, insideLengthN, underPCBs]);
    translate([insideWidth-bottomSlideWidth, 0,0])
        cube([bottomSlideWidth+nudge, insideLengthN, underPCBs]);
    translate([2*fitTolerance+cbWidth-nudge,0,0])
        cube([boardDivider+2*nudge,insideLengthN,underPCBs+max(cbPCBThickness,megaPCBThickness)+topSlideWidth]);
    translate([0,0,underPCBs+cbPCBThickness]) diagonalSlideLeft();
    translate([2*fitTolerance+cbWidth,0,underPCBs+cbPCBThickness]) diagonalSlideRight();
    translate([megaX-fitTolerance,0,underPCBs+megaPCBThickness]) diagonalSlideLeft();
    translate([megaX+fitTolerance+megaWidth,0,underPCBs+fitTolerance+megaPCBThickness]) diagonalSlideRight();
}

module mainSquare() {
}

boardMounts();
boards();