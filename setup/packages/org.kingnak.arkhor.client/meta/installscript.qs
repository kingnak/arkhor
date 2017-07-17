function Component() 
{ 
    // default constructor 
} 
 
Component.prototype.createOperations = function() 
{ 
    component.createOperations(); 
	component.addOperation("CreateShortcut", "@TargetDir@/arkhorclient.exe", "@StartMenuDir@/arkhorclient.lnk", "workingDirectory=@TargetDir@"); 
} 
