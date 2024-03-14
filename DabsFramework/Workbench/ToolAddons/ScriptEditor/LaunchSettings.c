enum GameLaunchType
{
	CLIENT = 1,
	SERVER = 2,
	BOTH   = 3, //CLIENT | SERVER
	OFFLINE= 8
};

// int.MAX prevents 'none' from showing up
enum YesNo
{
	NO = 0,
	YES = 2147483647,
};

enum BuilderType
{
	PBO_PROJECT = 0,
	ADDON_BUILDER,
};

class LaunchSettings: SerializableBase
{
	static const int VERSION = 2;
	
	static const string CLIENT_PROFILE_NAME = "client";
	static const string SERVER_PROFILE_NAME = "server";
	static const string BASE_LAUNCH_PARAMS = "-newErrorsAreWarnings=1 -doLogs -adminlog -scriptDebug=true -profile -resizeable -window -no_preload_vehicles -idleRender -noPause";
	
	protected string m_CurrentFileLocation;
	
	[Attribute("", "editbox", "Repository Directory")]
	string Repository;
	
	[Attribute("", "editbox", "Profile Directory (will be auto generated by the build tools)")]
	string Profiles;		
	
	[Attribute("", "editbox", "Mission Directory (will be auto generated by the build tools)")]
	string Missions;		
	
	[Attribute("", "editbox", "Mod Directory (will be auto generated by the build tools)")]
	string Mods;	
		
	[Attribute("", "combobox", "Launch", "", ParamEnumArray.FromEnum(GameLaunchType) )]
	int LaunchType;
			
	[Attribute("", "editbox", "Server password")]
	string ServerPassword;	
	
	[Attribute("", "editbox", "Map")]
	string Map;
		
	[Attribute("", "combobox", "Enable File Patching", "", ParamEnumArray.FromEnum(YesNo) )]
	bool FilePatching;	
	
	[Attribute("", "combobox", "Enable Deloginator", "", ParamEnumArray.FromEnum(YesNo) )]
	bool Deloginator;
	
	[Attribute("", "combobox", "Auto Close Game", "", ParamEnumArray.FromEnum(YesNo) )]
	bool AutoClose;
	
	[Attribute("", "combobox", "Disable Mod", "", ParamEnumArray.FromEnum(YesNo) )]
	bool DisableMod;
			
	void Save(string file)
	{
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file, FileMode.WRITE)) {
			return;
		}
		
		Write(serializer, VERSION);
		serializer.Close();
	}
	
	static LaunchSettings Load(string file)
	{		
		LaunchSettings settings = new LaunchSettings();
		settings.m_CurrentFileLocation = file;
		if (!FileExist(file)) {
			settings.LaunchType = GameLaunchType.BOTH;
			settings.Profiles = "P:\\Profiles";
			settings.Missions = "P:\\Missions";
			settings.Mods = "P:\\Mods";
			settings.Map = "ChernarusPlus";
			settings.FilePatching = true;
			settings.Deloginator = true;
			settings.AutoClose = true;
			settings.Save(file);
			return settings;
		}
		
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file, FileMode.READ)) {
			return null;
		}
		
		int version;
		if (!settings.Read(serializer, version)) {
			return null;
		}
		
		serializer.Close();
		return settings;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(VERSION);
		serializer.Write(Repository);
		serializer.Write(Profiles);
		serializer.Write(Missions);
		serializer.Write(Mods);
		serializer.Write(LaunchType);
		serializer.Write(ServerPassword);
		serializer.Write(Map);
		serializer.Write(FilePatching);
		serializer.Write(Deloginator);
		serializer.Write(AutoClose);
		serializer.Write(DisableMod);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!serializer.Read(version)) {
			return false;
		}
		
		if (!serializer.Read(Repository)) {
			return false;
		}
		
		if (!serializer.Read(Profiles)) {
			return false;
		}		
		
		if (!serializer.Read(Missions)) {
			return false;
		}		
		
		if (!serializer.Read(Mods)) {
			return false;
		}
			
		if (!serializer.Read(LaunchType)) {
			return false;
		}
											
		if (!serializer.Read(ServerPassword)) {
			return false;
		}		
				
		if (!serializer.Read(Map)) {
			return false;
		}		
		
		if (!serializer.Read(FilePatching)) {
			return false;
		}		

		if (!serializer.Read(Deloginator)) {
			return false;
		}		
		
		if (!serializer.Read(AutoClose)) {
			return false;
		}		
		
		if (version < 2) {
			return true;
		}
		
		if (!serializer.Read(DisableMod)) {
			return false;
		}
		
		return true;
	}
	
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		Save(m_CurrentFileLocation);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}