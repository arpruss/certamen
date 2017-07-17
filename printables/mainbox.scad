use <roundedsquare.scad>;
use <certamenbutton.scad>;
use <hershey.scad>;

DEMO = 0;
TOP = 1;
BOTTOM = 2;
WASHERS = 3;

mode = DEMO;

screwLength = 9.61;

// side labels
sideFont = "rowmans";
sideLabelSize = 12;

// main label
font = "Arial Black";
labelSize = 9;
label = "CERTAMEN";

visualizeBoards = mode == DEMO;

megaWidth = 53.34;
megaLength = 102.33;
megaHoles = [
		[  2.54, 15.24 ],
		[  50.8, 13.97 ],
		[  2.54, 90.17 ],
		[  50.8, 96.52 ]
		]; // from Kelly Egan's Arduino library
/* [[x,z,width,height],...] */
megaPCBThickness = 1.66;
megaFrontCutouts = [ [7, megaPCBThickness, 12.23, 10.82],
    [ 37.78, megaPCBThickness, 9.04, 11 ] ];
cbWidth = 54.9;
cbLength = 93; // 99.06 between edges of RJ-45 breakout board
cbPCBThickness = 1.7;
cbFrontCutouts = [
        [5.88,5.45,15.15,11.56] /* B */,
        [cbWidth-5.58-15.15,5.45,15.15,11.56] /* A */];
cbBackCutouts = [[cbWidth-5.58-15.15,5.45,15.15,11.56] /* C */];
cbHoles = [[4.04,4.19],[cbWidth-4.04,4.19],[4.04,84.6+4.19],[cbWidth-4.04,84.6+4.19]];

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
textDepth = 2; 

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
grilleReinforcement = 3.5;

alignerThickness = 1+fitTolerance;
alignerHeight = 4;

corner = 9; // must allow room for the speaker, as well as for the screw wells

pcbHolderThickness = 2;
pcbHolderHeight = underPCBs+pcbThickness+2;

spaceNeededForWires = 40;

boxHeight = underPCBs+max(spaceNeededForWires,speakerDiameter+2)+pcbThickness+topThickness+bottomThickness;

nudge = 0.001;
insideWidth = boardDivider+4*fitTolerance+megaWidth+cbWidth;
insideLength = max(megaLength,cbLength) + 2*fitTolerance;
boxWidth = insideWidth+2*corner+2*sideWallThickness;
boxLength = insideLength+2*sideWallThickness;

screenX = insideWidth/4;
screenY = insideLength/2;
clearX = 0.75*insideWidth;
clearY = insideLength/4;
modeSwitchNeckDiameter = 5.8;
modeSwitchOuterDiameter = 11.12;
modeSwitchNeck = 1.5;
modeX = 0.75*insideWidth;
modeY = 0.75*insideLength;
labelX = (screenX - screenHeight/2)*0.5-labelSize/2;
labelY = insideLength/2;

cbX = fitTolerance;
cbY = insideLength/2-cbLength/2;
megaX = 3*fitTolerance+cbWidth+boardDivider;
megaY = fitTolerance;
topZ = boxHeight-topThickness-bottomThickness;
speakerDiameter1 = speakerDiameter + 2 * fitTolerance;

screwTolerance = 0.125;
screwHoleSize = 2.6;
maxScrewLength = 10;
screwHoleSize1 = screwHoleSize + 2*screwTolerance;
screwPillarSize = 12;
screwHeadSize = 5.8;

speakerY = 0.5*insideLength;
speakerX = boxWidth-corner-2*sideWallThickness;
speakerZ = speakerDiameter1/2;

ventHeight=24;
ventWidth=insideWidth*0.4;
ventSpacing=2;

ventX=insideWidth*0.75;
ventZ=topZ-ventHeight/2-1;

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
        translate( [megaX,megaY,underPCBs] ) cube([megaWidth, megaLength, megaPCBThickness]);
        translate([megaX,megaY-cutoutLength,underPCBs]) cutouts(megaFrontCutouts, cutoutLength,extraCutout);
    }
    color("red") {
        translate( [cbX,cbY,underPCBs])
        cube([cbWidth, cbLength, cbPCBThickness]);
        translate([cbX,cbY-cutoutLength,underPCBs]) cutouts(cbFrontCutouts, cutoutLength,extraCutout);
        translate([cbX,cbY+cbLength,underPCBs]) cutouts(cbBackCutouts, cutoutLength,extraCutout);
    }
    color([0,.4,0]) {
        translate([screenX-screenHeight/2-extraCutout,screenY-screenWidth/2-extraCutout,topZ-5]) cube([screenHeight+2*extraCutout,screenWidth+2*extraCutout,cutoutLength+5]);
    }
    color([0,.8,0]) 
        translate([clearX,clearY,topZ-nudge]) arcadeButtonCylinder();
    color([0,0,0.5])
        translate([modeX,modeY,topZ-nudge]) { cylinder(d=modeSwitchNeckDiameter+2*extraCutout,h=topThickness+1);
    cylinder(d=modeSwitchOuterDiameter+2*extraCutout,h=modeSwitchNeck);
        }
    if (!visualize) {
        difference() {
            translate([speakerX-nudge,speakerY,speakerZ]) speakerGrille();
            translate([speakerX-2*nudge,speakerY-speakerDiameter,cutLine-grilleReinforcement/2]) cube([sideWallThickness*2,speakerDiameter*2,grilleReinforcement]);
        }
    }
}

module boardMounts() {
    render(convexity=2) {
    intersection() {
        union() {
            translate([cbX-fitTolerance-pcbHolderThickness,0,0]) cube([pcbHolderThickness,insideLength,pcbHolderHeight]);
            translate([cbX+cbWidth+fitTolerance,0,0]) cube([pcbHolderThickness,insideLength,pcbHolderHeight]);
            translate([megaX+megaWidth+fitTolerance,0,0]) cube([pcbHolderThickness,insideLength,pcbHolderHeight]);
        translate([0,insideLength-(megaLength-cbLength)/2,0]) cube([2*fitTolerance+cbWidth, pcbHolderThickness, underPCBs+pcbThickness+1]);
        translate([0,(megaLength-cbLength)/2-pcbHolderThickness,0]) cube([2*fitTolerance+cbWidth, pcbHolderThickness, underPCBs+pcbThickness+1]);
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

module labels() {
    translate([labelX,labelY,topZ+topThickness+nudge-textDepth]) linear_extrude(height=textDepth) rotate([0,0,-90]) text(label, font=font, size=labelSize, halign="center", valign="center");
}

module shell() {
    render(convexity=6)
    difference() {
        union() {
            difference() {
                translate([0,0,-bottomThickness+nudge]) box();
                box(height=boxHeight-bottomThickness-topThickness,inset=sideWallThickness);
            }
            screenScrews();
        }
        contents(visualize=false);
        screenScrews(holeOnly=true);
        translate([0,-sideWallThickness-nudge,0]) vent();
        translate([0,insideLength-nudge,0]) vent();
        joinScrewPillars(holeOnly=true);
        pcbScrews(megaX,megaY,megaHoles,holeOnly=true);
        pcbScrews(cbX,cbY,cbHoles,holeOnly=true);
        labels();
    }
}

module speakerMount() {
    height = topZ-cutLine;
    module strip() {
        cube([speakerMountXStickout+speakerMountThickness,speakerMountThickness,height]);
        translate([speakerMountXStickout,0,0])
        cube([speakerMountThickness,speakerMountYStickout+speakerMountThickness,height]);
    }
    translate([speakerX+nudge,speakerY,nudge])
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
    shell();
    if (visualizeBoards) contents(visualize=true);
    speakerMount();
    joinScrewPillars();
    portLabels();
}

module alignerPillar() {
    $fn=36;
    render(convexity=4)
    intersection() {
        difference() {
            union() {
                translate([0,0,cutLine]) cylinder(r=corner, h=topZ-cutLine+nudge);
                translate([0,0,cutLine-alignerHeight]) cylinder(r=corner-sideWallThickness-2*fitTolerance, h=alignerHeight+topZ-cutLine+nudge);
            }
            translate([0,0,cutLine-alignerHeight-nudge]) cylinder(r=corner-sideWallThickness-2*fitTolerance-alignerThickness, h=alignerHeight+topZ-cutLine+nudge);
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

module portLabel(s) {
    drawHersheyText(s, font=sideFont, valign="bottom", halign="center", size=sideLabelSize) cylinder(r1=sideLabelSize/5,r2=sideLabelSize/10,h=sideLabelSize/10);
}

cbFrontCutouts = [
        [5.88,5.45,15.15,11.56] /* B */,
        [cbWidth-5.58-15.15,5.45,15.15,11.56] /* A */];
cbBackCutouts = [[cbWidth-5.58-15.15,5.45,15.15,11.56] /* C */];

module portLabels() {
    z = underPCBs+3+cutoutTolerance+cbFrontCutouts[0][1]+cbFrontCutouts[0][3];
    translate([1+cbX+cbFrontCutouts[0][0]+0.5*cbFrontCutouts[0][2],-sideWallThickness+nudge,z])
    rotate([90,0,0]) {
        portLabel("B");
        translate([cbFrontCutouts[1][0]-cbFrontCutouts[0][0],0,0]) portLabel("A");
    }
    
    translate([cbX-1+cbBackCutouts[0][0]+0.5*cbBackCutouts[0][2],insideLength+sideWallThickness-nudge,z])
    rotate([0,0,180])
    rotate([90,0,0]) {
        portLabel("C");
    }
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

module screenWasher() {
    pillarSize = screwHoleSize1+5;
    length = screwLength+0.75-(pcbThickness+(screenDepth-1));
    render(convexity=2)
    difference() {
        cylinder(d=pillarSize, h=length, $fn=16);
        translate([0,0,-nudge]) cylinder(d=screwHoleSize1+1, h=length+2*nudge, $fn=16); 
    }
}

module pcbWasher() {
    pillarSize=screwHeadSize+2*fitTolerance;
    length = 2;
    render(convexity=2)
    difference() {
        cylinder(d=pillarSize, h=length, $fn=16);
        translate([0,0,-nudge]) cylinder(d=screwHoleSize1+1, h=length+2*nudge, $fn=16); 
    }
}

module washers() {
    for(i=[0:3]) translate([15*i,0,0]) screenWasher();
    for(i=[0:3]) translate([15*i,15,0]) pcbWasher();
}

module demo() {
    whole();
    alignerPillars();
}

if (mode == TOP) upperHalf();
else if (mode == BOTTOM) lowerHalf();
else if (mode == DEMO) demo();
else if (mode == WASHERS) washers();
