function Component() 
{ 
    // default constructor 
} 
 
Component.prototype.createOperations = function() 
{ 
    component.createOperations(); 
	component.addOperation("CreateShortcut", "@TargetDir@/arkhorserver.exe", "@StartMenuDir@/arkhorserver.lnk", "workingDirectory=@TargetDir@"); 
} 
