
use <roundedsquare.scad>;
use <tubemesh.scad>;
use <certamenbutton.scad>;

DEMO = 0;
TOP = 1;
BOTTOM = 2;

mode = BOTTOM;

// TODO: 
//  cut up
//  screw wells for joining halves

visualizeBoards = mode == DEMO;
megaWidth = 53.34;
megaLength = 101.6;
megaHoles = [
		[  2.54, 15.24 ],
		[  50.8, 13.97 ],
		[  2.54, 90.17 ],
		[  50.8, 96.52 ]
		]; // from Kelly Egan's Arduino library
/* [[x,z,width,height],...] */
megaPCBThickness = 1.66;
megaFrontCutouts = [ [7, megaPCBThickness, 12.23, 10.82],
    [ megaWidth-9.04-6.7, megaPCBThickness, 9.04, 11.08 ] ];
cbWidth = 54.9;
cbLength = 99.06; // between edges of RJ-45 breakout board
cbPCBThickness = 1.7;
cbFrontCutouts = [[5.88,5.45,15.15,11.56],[cbWidth-5.58-15.15,5.45,15.15,11.56]];
cbBackCutouts = [[cbWidth-5.58-15.15,5.45,15.15,11.56]];
cbHoles = [[4.04,7.5],[cbWidth-4.04,7.5],[4.04,cbLength-7.5],[cbWidth-4.04,cbLength-7.5]];

pcbThickness = max(cbPCBThickness,megaPCBThickness);

mountExtra = 2;

underPCBs = 5;

cutLine = underPCBs + 0.5*( cbFrontCutouts[0][1] + megaPCBThickness + max([for(c=megaFrontCutouts) c[3]]) );

fitTolerance = 0.4;
cutoutTolerance = 1;

boardDivider = 2;
sideWallThickness = 1.25;
bottomThickness = 1.5;
topThickness = 3.2; // match Certamen button

screenScrewHorizontalSpacing = 74.8;
screenScrewVerticalSpacing = 31.2;
screenWidth = 71.21;
screenHeight = 24.04; 
screenDepth = 7; 

screenScrewWellDepth = 7.97;
speakerDiameter = 39.2;
speakerMountYStickout = 1.86; 
speakerMountXStickout = 5; 
speakerMountThickness = 1.5;
grilleSolidWidth = 2;
grilleHoleWidth = 2.5;

alignerThickness = 1+fitTolerance;
alignerHeight = 4;

corner = 9; // must allow room for the speaker, as well as for the screw wells

pcbHolderThickness = 2;
pcbHolderHeight = underPCBs+pcbThickness+2;

boxHeight = speakerDiameter+underPCBs+2+pcbThickness+topThickness+bottomThickness;

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
cbY = fitTolerance;
megaX = 3*fitTolerance+cbWidth+boardDivider;
megaY = fitTolerance;
topZ = boxHeight-topThickness-bottomThickness;
speakerDiameter1 = speakerDiameter + 2 * fitTolerance;

screwTolerance = 0.125;
screwHoleSize = 2.6;
maxScrewLength = 10;
screwHoleSize1 = screwHoleSize + 2*screwTolerance;
screwPillarSize = 12;
screwHeadSize = 5.35;

speakerY = 0.5*insideLength;
speakerX = boxWidth-corner-2*sideWallThickness;
speakerZ = speakerDiameter1/2;

ventHeight=18;
ventWidth=insideWidth*0.75;
ventSpacing=2;

ventX=insideWidth/2;
ventZ=topZ-10;

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
        translate([-nudge, y, -h])
        cube([sideWallThickness+2*nudge, grilleHoleWidth, 2*h]);
    }
}

module contents(visualize=true) {
    cutoutLength = visualize?10:100;
    extraCutout = visualize?0:cutoutTolerance;
    color("blue") {
        translate( [megaX,fitTolerance,underPCBs] ) cube([megaWidth, megaLength, megaPCBThickness]);
        translate([megaX,fitTolerance-cutoutLength,underPCBs]) cutouts(megaFrontCutouts, cutoutLength,extraCutout);
    }
    color("red") {
        translate( [cbX,fitTolerance,underPCBs])
        cube([cbWidth, cbLength, cbPCBThickness]);
        translate([cbX,fitTolerance-cutoutLength,underPCBs]) cutouts(cbFrontCutouts, cutoutLength,extraCutout);
        translate([cbX,fitTolerance+cbLength,underPCBs]) cutouts(cbBackCutouts, cutoutLength,extraCutout);
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

module boardMounts() {
    render(convexity=2) {
    intersection() {
        union() {
            translate([cbX-fitTolerance-pcbHolderThickness,0,0]) cube([pcbHolderThickness,insideLength,pcbHolderHeight]);
            translate([cbX+cbWidth+fitTolerance,0,0]) cube([pcbHolderThickness,insideLength,pcbHolderHeight]);
            translate([megaX+megaWidth+fitTolerance,0,0]) cube([pcbHolderThickness,insideLength,pcbHolderHeight]);
        translate([0,insideLength-(megaLength-cbLength),0]) cube([2*fitTolerance+cbWidth, megaLength-cbLength, pcbHolderHeight]);
        }        
        box();
    }    
    pcbScrews(megaX,megaY,megaHoles);
    pcbScrews(cbX,cbY,cbHoles);
}
}

module screwPillar(height, taper=true, holeOnly=false) {
    difference() {
        if (!holeOnly) cylinder(d1=screwPillarSize, d2=taper ? screwHoleSize1+4 : screwPillarSize, h=height, $fn=16);
        translate([0,0,-bottomThickness-nudge])
        cylinder(d=screwHoleSize1, h=height+bottomThickness+2*nudge, $fn=16);
    }
}

module pcbScrews(x,y,positions,holeOnly=false) {
    for(pos=positions) {
        translate([x+pos[0],y+pos[1],0]) screwPillar(underPCBs, taper=true,holeOnly=holeOnly);
    }
}

module box(height=boxHeight,inset=0) {
    linear_extrude(height=height) translate([-corner-sideWallThickness+inset,-sideWallThickness+inset]) roundedSquare([boxWidth-2*inset, boxLength-2*inset], radius=corner-inset,$fn=36);
}

module screenScrew(holeOnly=false) {
    pillarSize = screwHoleSize1+4;
    render(convexity=2)
    translate([0,0,topZ-(screenDepth-topThickness)-nudge])
    difference() {
        if (!holeOnly) cylinder(d=pillarSize, h=screenDepth, $fn=16);
        translate([0,0,-nudge]) cylinder(d=screwHoleSize1, h=screenDepth-1, $fn=16);
    }
}

module screenScrews(holeOnly=false) {
    dx = screenScrewVerticalSpacing / 2;
    dy = screenScrewHorizontalSpacing / 2;

    translate([screenX-dx,screenY-dy]) screenScrew(holeOnly=holeOnly);
    translate([screenX+dx,screenY-dy]) screenScrew(holeOnly=holeOnly);
    translate([screenX-dx,screenY+dy]) screenScrew(holeOnly=holeOnly);
    translate([screenX+dx,screenY+dy]) screenScrew(holeOnly=holeOnly);
}

module shell() {
    render(convexity=6)
    difference() {
        translate([0,0,-bottomThickness+nudge]) box();
        box(height=boxHeight-bottomThickness-topThickness,inset=sideWallThickness);
        contents(visualize=false);
        screenScrews(holeOnly=true);
        translate([0,-sideWallThickness-nudge,0]) vent();
        translate([0,insideLength-nudge,0]) vent();
        joinScrewPillars(holeOnly=true);
        pcbScrews(megaX,megaY,megaHoles,holeOnly=true);
        pcbScrews(cbX,cbY,cbHoles,holeOnly=true);
    }
}

module speakerMount() {
    height = nudge+topZ-speakerZ+10;
    module strip() {
        cube([speakerMountXStickout+speakerMountThickness,speakerMountThickness,height]);
        translate([speakerMountXStickout,0,0])
        cube([speakerMountThickness,speakerMountYStickout+speakerMountThickness,height]);
    }
    translate([speakerX+nudge,speakerY,0])
    rotate([0,0,180]) {
        translate([0,0,topZ-height]) {
        translate([0,-speakerDiameter1/2-speakerMountThickness,0]) strip();
        translate([0,speakerDiameter1/2+speakerMountThickness,0]) mirror(v=[0,1,0]) strip();
        }
        topHolderHeight = (topZ-speakerZ)-speakerDiameter1/2;
        translate([0,-5, topZ-topHolderHeight])
        cube([speakerMountXStickout, 10, topHolderHeight+nudge]);
    }
}

module vent() {
    translate([ventX-ventWidth/2,-nudge,ventZ-ventHeight/2])
    for (x=[0:2*ventSpacing:ventWidth]) {
        translate([x,0,0])
        cube([ventSpacing,4*nudge+sideWallThickness,ventHeight]);
    }
}

module joinScrewPillar(holeOnly=false) {
    $fn = 16;
    render(convexity=2)
    difference() {
        if(!holeOnly) translate([0,0,-bottomThickness]) cylinder(d=corner,h=bottomThickness+2*nudge+topZ);
        union() {
            cylinder(d=screwHoleSize1,h=maxScrewLength+cutLine);
            cylinder(d=screwHoleSize1+1,h=cutLine);
            translate([0,0,-bottomThickness-nudge])
            cylinder(d=screwHeadSize+2*fitTolerance,h=cutLine-3+bottomThickness);
        }
    }
}

module joinScrewPillars(holeOnly=false) {
    module pair() {
    translate([-corner/2-sideWallThickness,0,0]) joinScrewPillar(holeOnly=holeOnly);
    translate([insideWidth+corner/2+sideWallThickness,0,0]) joinScrewPillar(holeOnly=holeOnly);
    }
    translate([0,corner*2,0]) pair();
    translate([0,insideLength-corner*2,0]) pair();
}

module whole() {
    boardMounts();
    screenScrews();
    shell();
    if (visualizeBoards) contents(visualize=true);
    speakerMount();
    joinScrewPillars();
}

module alignerPillar() {
    $fn=36;
    render(convexity=4)
    intersection() {
        difference() {
            union() {
                translate([0,0,cutLine]) cylinder(r=corner, h=topZ-cutLine+nudge);
                translate([0,0,cutLine-alignerHeight]) cylinder(r=corner-sideWallThickness-fitTolerance, h=alignerHeight+topZ-cutLine+nudge);
            }
            translate([0,0,cutLine-alignerHeight-nudge]) cylinder(r=corner-sideWallThickness-fitTolerance-alignerThickness, h=alignerHeight+topZ-cutLine+nudge);
        }
       translate([-corner, -corner, cutLine-alignerHeight-nudge]) 
            cube([corner,corner,alignerHeight+topZ-cutLine+nudge]);
    }
}

module alignerPillars() {
    translate([-sideWallThickness,corner-sideWallThickness,0])
    alignerPillar();
    translate([insideWidth+sideWallThickness,corner-sideWallThickness,0])
    rotate([0,0,90])
    alignerPillar();
    translate([-sideWallThickness,insideLength-corner+sideWallThickness,0])
    rotate([0,0,-90])
    alignerPillar();
    translate([insideWidth+sideWallThickness,insideLength-corner+sideWallThickness,0])
    rotate([0,0,180])
    alignerPillar();
}

module lowerHalf() {
    render(convexity=5)
    intersection() {
        whole();
        translate([-20,-20,-20]) cube([boxWidth+40,boxLength+40,cutLine+20]);
    }
}

module upperHalf() {
    rotate([180,0,0]) 
    render(convexity=5) {
        intersection() {
            whole();
            translate([-20,-20,cutLine]) cube([boxWidth+40,boxLength+40,topThickness+topZ-cutLine+20]);
        }
        alignerPillars();
    }
}

module demo() {
    whole();
    alignerPillars();
}

if (mode == TOP) upperHalf();
else if (mode == BOTTOM) lowerHalf();
else demo();
