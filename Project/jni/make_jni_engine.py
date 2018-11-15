def ModuleFileList(moduleName, msvcProjectFileName, androidFileName):
	from xml.dom.minidom import parse;
	import xml.dom.minidom;
	
	DOMTree = xml.dom.minidom.parse(msvcProjectFileName);
	collection = DOMTree.documentElement;
	
	file = open(androidFileName, "w");
	file.write("%s := \\\n" % moduleName);
	
	itemGroups = collection.getElementsByTagName("ItemGroup");
	for itemGroup in itemGroups:
		files = itemGroup.getElementsByTagName("ClCompile");
		for index in range(files.length):
			'''
			isExcluded = False;
			
			excluded = files[index].getElementsByTagName("ExcludedFromBuild");
			for index in range(excluded.length):
				condition = excluded[index].getAttribute("Condition");
				if condition.find("Library") != "-1":
					isExcluded = True;
					break;
			
			if isExcluded == True:
				continue;
			'''
			
			fileName = files[index].getAttribute("Include");
			fileName = fileName.replace("\\", "/");
			if index < files.length - 1:
				fileName += " \\";
			file.write(fileName + "\n");
	
	file.close();
	return;
	
ModuleFileList("ENGINE_SRC_FILES", "../msvc/Engine.vcxproj", "./ENGINE_SRC_FILES.mk");
