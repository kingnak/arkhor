function Component() 
{ 
    // default constructor 
} 
 
Component.prototype.createOperations = function() 
{ 
    component.createOperations(); 
	component.addOperation("CreateShortcut", "@TargetDir@/arkhorserver_test.exe", "@StartMenuDir@/arkhorserver_test.lnk", "workingDirectory=@TargetDir@"); 
} 
