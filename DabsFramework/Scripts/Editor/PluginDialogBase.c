class PluginDialogBase: WorkbenchPlugin
{	
	// Sizes the dialog to max size without putting a scroll bar on the bottom
	static const string DIALOG_TAB_SIZE = "\t\t\t\t\t\t\t\t\t";
	static const string PATH_SEPERATOR = "/";
	static const string PATH_SEPERATOR_ALT = "\\";
	static const string DEFAULT_EXTENSION = ".c";
	static const ref array<string> LOG_FILE_TYPES = {".log", ".rpt", ".adm", ".mdmp"};
	
	protected ScriptEditor m_ScriptEditor = Workbench.GetModule("ScriptEditor");
	protected ResourceBrowser m_ResourceBrowser = Workbench.GetModule("ResourceBrowser");
	
	void ErrorDialog(string error)
	{
		Workbench.Dialog(string.Format("Error: %1", Type()), error);
	}
	
	static string GetPrefix()
	{
		// Dump the stack of this file, that way we can see exactly where we're being launched from
		string stack;
		DumpStackString(stack);
		
		array<string> stack_newline_split = {};
		stack.Split("\n", stack_newline_split);
		if (stack_newline_split.Count() == 0) {
			return string.Empty;
		}
		
		string tokens[32];
		int count = stack_newline_split[stack_newline_split.Count() - 1].ParseString(tokens);
		if (count < 4) {
			return string.Empty;
		}
		
		string prefix = tokens[3].Trim();
		return prefix;
	}
	
	static string GetSourceDataDirectory()
	{
		string abs;
		Workbench.GetAbsolutePath("$SourceData:", abs);
		return abs;
	}
	
	static string GetWorkbenchDirectory()
	{
		string workbench_dir;
		Workbench.GetCwd(workbench_dir);
		return workbench_dir;
	}
	
	static string GetRootDirectory()
	{
		string root_dir;
		Workbench.GetAbsolutePath(string.Empty, root_dir);
		return root_dir;
	}
	
	static string GetAbsolutePath(string path)
	{
		path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		
		string absolute_path = GetRootDirectory();
		if (path.Length() == 0) {
			return absolute_path;
		}
		
		// Sanitize initial path. GetRootDirectory does 
		if (absolute_path[0] == PATH_SEPERATOR) {
			absolute_path[0] = string.Empty;
		}
		
		return absolute_path + PATH_SEPERATOR + path;
	}
	
	static string GetDirectory(string path)
	{
		path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		
		array<string> path_split = {};
		path.Split(PATH_SEPERATOR, path_split);
		string directory;
		for (int i = 0; i < path_split.Count(); i++) {
			if (path_split[i].Contains(".")) {
				// pop upon finding file. Substring because we added a seperator to the end last loop
				return directory;
			}
			
			directory += path_split[i];
			if (i != path_split.Count() - 1) {
				directory += PATH_SEPERATOR;
			}
		}
		
		return directory;
	}
	
	static string GetFile(string path)
	{
		path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		
		array<string> path_split = {};
		path.Split(PATH_SEPERATOR, path_split);
		
		if (path_split.Count() == 0) {
			return string.Empty;
		}
		
		return path_split[path_split.Count() - 1];
	}
	
	static FileHandle CreateFile(string file)
	{		
		// Creates needed directories
		string absolute_file_rebuild;
		array<string> absolute_file_split = {};
		file.Split(PATH_SEPERATOR, absolute_file_split);
		for (int i = 0; i < absolute_file_split.Count(); i++) {
			if (absolute_file_split[i].Contains(".")) {
				break;
			}
			
			absolute_file_rebuild += absolute_file_split[i];
													// drive specifier
			if (!FileExist(absolute_file_rebuild) && !absolute_file_split[i].Contains(":")) {
				Workbench.RunCmd("cmd /c mkdir " + absolute_file_rebuild);
			}
			
			absolute_file_rebuild += PATH_SEPERATOR_ALT;
		}
		
		// Create the file
		int result = Workbench.RunCmd(string.Format("cmd /c copy NUL \"%1\"", file), true);
		if (result != 0) {
			return null;
		}
				
		return OpenFile(file, FileMode.WRITE);
	}
	
	static int RunCommandPrompt(string cmd, bool wait = false)
	{
		return Workbench.RunCmd(string.Format("cmd /c %1", cmd), wait);
	}
	
	static int MakeSymLink(string source, string target)
	{
		if (!FileExist(target)) {
			return RunCommandPrompt(string.Format("mklink /j \"%2\" \"%1\"", source, target), true);
		}
		
		return 0;
	}
	
	static void KillTask(string task_name)
	{
		Workbench.RunCmd(string.Format("taskkill /F /IM %1 /T", task_name));
	}
	
	static void CleanLogFolder(string folder)
	{
		foreach (string file_type: LOG_FILE_TYPES) {
			Workbench.RunCmd(string.Format("forfiles -p %2 /m *%1 /s /d -1 -c \"cmd /c del @path\"", file_type, folder));
		}
	}
}