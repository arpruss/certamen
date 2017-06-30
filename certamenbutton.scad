// print at 0.20 mm layers, PLA, 25% infill
includeBase = 0; // [1:yes, 0:no]
includeMain = 0; // [1:yes, 0:no]
includeScrewTemplate = 1; // [1:yes, 0:no]
height = 40; 
innerDiameter = 59.65;
label = "C3";
numberOfCablePorts = -1; // [-1:automatic, 0:none, 1:1, 2:2]
font = "Arial Black:style=Bold";
labelSize = 9;
textPositionAdjustment = 1;
baseThickness = 5.8;
screwHoleSize = 2.6;
screwPillarDiameter = 12;
cablePortThickness = 3; 

holeDiameterMain = 28.88; 
tolerance = 0.125;
snapThickness = 0.25;
snapWidth = 7.25;
snapThickening = 0.5;
antiRotation = 2.3; 

wallThickness = 1.25;
topThickness = 3.2; 
textDepth = 2; 
chamfer = 3;
cableHoleDiameter = 4.9; 
cableHoleBottomSquish = 1;
screwCountersinkDepth = 2;
screwCountersinkDiameter = 5;
screwDepth = 15;

// The cable hole cover is usually not needed, but if you
// printed a two-hole version when you needed a one-hole
// one, this will save you a reprint.
includeCableHoleCover = 0; // [1:yes, 0:no]

module dummy() {}

cablePorts = numberOfCablePorts < 0 ? 
                (label[1] == "4" ? 1 : 2) : 
                numberOfCablePorts;

baseThicknessAdj = baseThickness + tolerance;

outerDiameter = innerDiameter + 2*wallThickness + 2*tolerance; 
nudge = 0.01;

module chamferedCylinder(h=height, d=outerDiameter, chamfer=chamfer) {
    cylinder(h=chamfer*1.5+nudge, d1=d-2*chamfer, d2=d);
    translate([0,0,chamfer*1.5]) cylinder(h=h-chamfer*1.5, d=d);
}

module screws() {
    for(angle=[0:180:180]) rotate([0,0,angle]) {
            translate([-outerDiameter/2+wallThickness/2+screwPillarDiameter/2,-screwPillarDiameter/2-cableHoleDiameter/2-tolerance,-nudge]) children();
            translate([-outerDiameter/2+wallThickness/2+screwPillarDiameter/2,screwPillarDiameter/2+cableHoleDiameter/2+tolerance,-nudge]) children();
    }
}

module screwPillar() {
    intersection() {
        difference() {
            cylinder(d=screwPillarDiameter, h=height-baseThicknessAdj,$fn=12);
            translate([0,0,-nudge]) cylinder(d=screwHoleSize+2*tolerance, h=height-baseThicknessAdj+2*nudge, $fn=12);
        }
        rotate([0,0,180])
        translate([0,screwPillarDiameter/2,0])
        rotate([90,0,0])
        linear_extrude(height=screwPillarDiameter)
        polygon([[screwPillarDiameter/2, height-baseThicknessAdj-screwDepth-screwPillarDiameter], [-screwPillarDiameter/2, height-baseThicknessAdj-screwDepth], [-screwPillarDiameter/2, height], [screwPillarDiameter/2,height] ]);
    }
}

module cable(tolerance) {
    if (cablePorts) {
        translate([cablePorts==1 ? -(1.5*outerDiameter+2*cablePortThickness)/2 : 0,0,height-cableHoleDiameter/2-tolerance-baseThicknessAdj+cableHoleBottomSquish])
        rotate([0,90,0]) {
            translate([-cableHoleDiameter/2-tolerance-baseThicknessAdj/2,0,0])
            cube([cableHoleDiameter+2*tolerance+baseThicknessAdj,cableHoleDiameter+2*tolerance,1.5*outerDiameter+2*cablePortThickness], center=true);
            translate([0,0,-0.75*outerDiameter-cablePortThickness]) 
            cylinder(h=1.5*outerDiameter+2*cablePortThickness,d=cableHoleDiameter+2*tolerance);
    }
}
}



module portCover() {
    translate([0,cableHoleDiameter/2+tolerance+cablePortThickness,height])
    rotate([90,0,0])
    linear_extrude(height=cableHoleDiameter+2*tolerance+2*cablePortThickness)
    polygon([[-cablePortThickness,0],[0,0],[0,-baseThicknessAdj-cableHoleDiameter+cableHoleBottomSquish-2*tolerance-cablePortThickness*2],[-cablePortThickness,-baseThicknessAdj-cableHoleDiameter-cablePortThickness+cableHoleBottomSquish-2*tolerance]]);
}


module mainCylinder() {
    render(convexity=4)
    difference() {
        union() {
            intersection() {
                chamferedCylinder();
                union() {
                    difference() {
                        chamferedCylinder();
                        translate([0,0,topThickness]) chamferedCylinder(h=height-topThickness+nudge, d=outerDiameter-2*wallThickness);
                    }
                    screws() screwPillar();
                for (angle=[90:180:270]) 
                rotate([0,0,angle]) translate([0,-snapWidth/2,-nudge])
cube([holeDiameterMain/2+tolerance+snapThickness+2,snapWidth,topThickness+snapThickening]);
                }
            }
            if (cablePorts)
            for(angle=[0:180:cablePorts==1?0:180]) rotate([0,0,angle]) translate([-outerDiameter/2+wallThickness/2,0,0]) portCover();
        }
        cable(tolerance);
        rotate([0,0,90]) {
            translate([0,0,-nudge]) cylinder(h=height, d=holeDiameterMain+2*tolerance);
            for (angle=[0:180:180]) 
                rotate([0,0,angle]) 
                    translate([0,-snapWidth/2,-nudge])
                    cube([holeDiameterMain/2+tolerance+snapThickness,snapWidth,2*nudge+topThickness+snapThickening]);
            for (angle=[45:90:360-45]) rotate([0,0,angle]) translate([holeDiameterMain/2+tolerance,0,-nudge]) cylinder(d=antiRotation+2*tolerance,h=topThickness+2*nudge,$fn=12);
        }
        translate([0,holeDiameterMain/4+(outerDiameter/4-chamfer/2)-.08*labelSize+textPositionAdjustment,textDepth-nudge]) rotate([180,0,0]) linear_extrude(height=textDepth) text(label, font=font, size=labelSize, halign="center", valign="center");
    }
}

module cableHoleCover() {
    cable(0);
}

module base(template=false) {
    difference() {
            cylinder(d=innerDiameter-2*tolerance, h=template?0.8:baseThickness);
            screws() {
                cylinder(d=screwHoleSize+2*tolerance,h=baseThicknessAdj+2*nudge,$fn=12);
                if (!template) {
                    translate([0,0,baseThickness-screwCountersinkDepth-tolerance])
                    cylinder(d=screwCountersinkDiameter+2*tolerance,h=screwCountersinkDepth+tolerance+nudge,$fn=12);
                }
            }
            if (template)
                translate([0,0,-nudge]) cylinder(d=0.7*innerDiameter-2*tolerance, h=0.9);

        }
        if (!template && cablePorts)
        for(angle=[0:180:cablePorts==1?0:180]) rotate([0,0,angle]) translate([-innerDiameter/2+tolerance-(cablePortThickness+wallThickness-2*tolerance),-cableHoleDiameter/2-tolerance,0]) cube([cablePortThickness+wallThickness+nudge+innerDiameter/4,cableHoleDiameter-2*tolerance, baseThickness]);
}

$fn = 72;
if (includeMain)
 mainCylinder();

if (includeBase) 
{
render(convexity=3)
    translate([outerDiameter+cablePortThickness+10,0,0]) 
    base();
}

if (includeScrewTemplate) 
{
render(convexity=3)
    translate(2*[outerDiameter+cablePortThickness+10,0,0]) 
    base(template=true);
}

if (includeCableHoleCover) {
    render(convexity=0)
    rotate([0,-90,0])
    intersection() {
        cableHoleCover();
        translate([0,-20,0]) cube([cablePortThickness,40,height]);
    }
}
