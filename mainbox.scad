use <roundedsquare.scad>;
use <tubemesh.scad>;
use <certamenbutton.scad>;

// TODO: 
//  speaker rails
//  cut up
//  screw wells for screen
//  screw wells for joining halves

includeBoards = false;
megaWidth = 53.34;
megaLength = 101.6;
megaPCBThickness = 1.66;
megaHoles = [ //[  50.8, 13.97 ],
		[  50.8, 96.52 ]];
/* [[x,z,width,height],...] */
megaFrontCutouts = [ [7, megaPCBThickness, 12.23, 10.82],
    [ megaWidth-9.04-6.7, megaPCBThickness, 9.04, 11.08 ] ];
cbWidth = 54.9;
cbLength = 99.06; // between edges of RJ-45 breakout board
cbPCBThickness = 1.7;
cbFrontCutouts = [[5.88,5.45,15.15,11.56],[cbWidth-5.58-15.15,5.45,15.15,11.56]];
cbBackCutouts = [[cbWidth-5.58-15.15,5.45,15.15,11.56]];
cbHoles = [[4.04,cbLength-7.5]];
maxPCBThickness = max(megaPCBThickness, cbPCBThickness);
mountExtra = 2;

boardDivider = 3;
slideBackbone = 3;
underPCBs = 5;

fitTolerance = 0.4;
cutoutTolerance = 1;
bottomSlideWidth = 1.82;
topSlideWidth = 1.75;

sideWallThickness = 1.25;
bottomThickness = 1.5;
topThickness = 3.2; // match Certamen button

screenScrewHorizontalSpacing = 74.8;
screenScrewVerticalSpacing = 31.2;
screenWidth = 71.21;
screenHeight = 24.04; 
// make sure screendepth fits well 

screenScrewWellDepth = 7.97;
speakerDiameter = 39.2;
speakerMountYStickout = 1.86; 
speakerMountXStickout = 5; 
speakerMountThickness = 1.5;
grilleSolidWidth = 2;
grilleHoleWidth = 2.5;

corner = 9;

boxHeight = speakerDiameter+underPCBs+2+maxPCBThickness+topThickness+bottomThickness;

nudge = 0.001;
insideWidth = boardDivider+4*fitTolerance+megaWidth+cbWidth;
insideLength = max(megaLength,cbLength) + 2*fitTolerance;
boxWidth = insideWidth+2*corner+2*sideWallThickness;
boxLength = insideLength+2*sideWallThickness;

screenX = insideWidth/4;
screenY = insideLength/2;
clearX = 0.75*insideWidth;
clearY = insideLength/4;
modeDiameter = 5.8;
modeX = 0.75*insideWidth;
modeY = 0.75*insideLength;

cbX = fitTolerance;
megaX = 3*fitTolerance+cbWidth+boardDivider;
topZ = boxHeight-topThickness-bottomThickness;
speakerDiameter1 = speakerDiameter + 2 * fitTolerance;

speakerY = 0.5*insideLength;
speakerX = boxWidth-corner-2*sideWallThickness;
speakerZ = (topZ + maxPCBThickness+underPCBs)/2;

module cutouts(cutouts, length, extra) {
    for (c=cutouts) {
        x = c[0]-extra;
        z = c[1]-extra;
        w = c[2]+2*extra;
        h = c[3]+2*extra;
        translate([x,0,z]) cube([w,length,h]);
    }
}

module speakerGrille() {
    r = speakerDiameter/2-speakerMountYStickout;
    for (y=[-r:grilleSolidWidth+grilleHoleWidth:r]) {
        y1 = y+grilleHoleWidth/2;
        h = sqrt(r*r-y1*y1);
        translate([0, y, -h])
        cube([sideWallThickness+2*nudge, grilleHoleWidth, 2*h]);
    }
}

module contents(visualize=true) {
    cutoutLength = visualize?10:100;
    extraCutout = visualize?0:cutoutTolerance;
    color("blue") {
        translate( [megaX,fitTolerance,underPCBs] ) cube([megaWidth, megaLength, megaPCBThickness]);
        translate([megaX,fitTolerance+nudge-cutoutLength,underPCBs-nudge]) cutouts(megaFrontCutouts, cutoutLength,extraCutout);
    }
    color("red") {
        translate( [cbX,fitTolerance,underPCBs])
        cube([cbWidth, cbLength, cbPCBThickness]);
        translate([cbX,fitTolerance+nudge-cutoutLength,underPCBs-nudge]) cutouts(cbFrontCutouts, cutoutLength,extraCutout);
        translate([cbX,fitTolerance+cbLength-nudge,underPCBs-nudge]) cutouts(cbBackCutouts, cutoutLength,extraCutout);
    }
    color([0,.4,0]) {
        translate([screenX-screenHeight/2-extraCutout,screenY-screenWidth/2-extraCutout,topZ-1]) cube([screenHeight+2*extraCutout,screenWidth+2*extraCutout,cutoutLength]);
    }
    color([0,.8,0]) 
        translate([clearX,clearY,topZ-nudge]) arcadeButtonCylinder();
    color([0,0,0.5])
        translate([modeX,modeY,topZ-nudge]) cylinder(d=modeDiameter+2*extraCutout,h=topThickness+1);
    if (!visualize) {
        translate([speakerX,speakerY-nudge,speakerZ]) speakerGrille();
    }
}

mountHeight = underPCBs+maxPCBThickness+topSlideWidth+mountExtra;

module boardMounts() {
    module diagonalSlideLeft() {
        section1 = [ [0,0,0], [topSlideWidth,0,topSlideWidth], [topSlideWidth,0,topSlideWidth+mountExtra],[0,0,topSlideWidth+mountExtra] ];
        section2 = [for (v=section1) v+[0,insideLength,0]];
        
        leftPointsAndFaces = 
            pointsAndFaces([ section1, section2]);
        polyhedron(points=leftPointsAndFaces[0], faces=leftPointsAndFaces[1]);
    }
    
    module diagonalSlideRight() {
        mirror([1,0,0]) diagonalSlideLeft();
    }
    
    render(convexity=2)
    intersection() {
        union() {
        translate([nudge-slideBackbone,0,0])
            cube([slideBackbone,insideLength,mountHeight]);
        translate([insideWidth-nudge,0,0])
            cube([slideBackbone,insideLength,mountHeight]);

        cube([bottomSlideWidth, insideLength, underPCBs]);
        translate([2*fitTolerance+cbWidth-bottomSlideWidth,0,0]) 
            cube([bottomSlideWidth*2+boardDivider, insideLength, underPCBs]);
        translate([insideWidth-bottomSlideWidth, 0,0])
            cube([bottomSlideWidth+nudge, insideLength, underPCBs]);
        translate([2*fitTolerance+cbWidth-nudge,0,0])
            cube([boardDivider+2*nudge,insideLength,mountHeight]);
        translate([0,0,underPCBs+cbPCBThickness]) diagonalSlideLeft();
        translate([2*fitTolerance+cbWidth,0,underPCBs+cbPCBThickness]) diagonalSlideRight();
        translate([megaX-fitTolerance,0,underPCBs+megaPCBThickness]) diagonalSlideLeft();
        translate([megaX+fitTolerance+megaWidth,0,underPCBs+megaPCBThickness]) diagonalSlideRight();
        translate([0,insideLength-(megaLength-cbLength),0]) cube([2*fitTolerance+cbWidth, megaLength-cbLength, underPCBs+cbPCBThickness+topSlideWidth]);
        }
        box(inset=sideWallThickness+fitTolerance);
    }
}

module box(height=boxHeight,inset=0) {
    linear_extrude(height=height) translate([-corner-sideWallThickness+inset,-sideWallThickness+inset]) roundedSquare([boxWidth-2*inset, boxLength-2*inset], radius=corner-inset);
}

module shell() {
    render(convexity=6)
    difference() {
        translate([0,0,-bottomThickness]) box();
        box(height=boxHeight-bottomThickness-topThickness,inset=sideWallThickness);
        contents(visualize=false);
    }
}

module bottom() {
    boardMounts();
    %shell();
    if (includeBoards) contents(visualize=true);
}

%shell();
bottom();

// todo: screws; front empty, back full