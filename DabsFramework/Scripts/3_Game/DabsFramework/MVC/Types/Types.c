// 0: Property Name
// 1: Property Type
class PropertyTypeHashMap: map<string, typename>
{
	void PropertyTypeHashMap(typename type)
	{
		for (int i = 0; i < type.GetVariableCount(); i++) {
			this[type.GetVariableName(i)] = type.GetVariableType(i);
		}	
	}
	
	void RemoveType(typename removed_type)
	{
		PropertyTypeHashMap hash_map = new PropertyTypeHashMap(removed_type);
		foreach (string name, typename type: hash_map) {
			Remove(name);
		}
	}
}


// 0: Source Widget
// 1: View Binding
typedef map<Widget, ViewBinding> ViewBindingHashMap;

// 0: View Binding
typedef set<ViewBinding> ViewBindingSet;

// 0: Property Name
// 1: View Binding Set
class DataBindingHashMap: map<string, autoptr ViewBindingSet>
{	
	void InsertView(ViewBinding view)
	{
		TStringArray names = {view.Binding_Name, view.Selected_Item};
		foreach (string name: names) {			
			ViewBindingSet view_set = Get(name);
			if (view_set) {
				view_set.Insert(view);
				continue;
			} 
		
			view_set = new ViewBindingSet();
			view_set.Insert(view);
			Insert(name, view_set);
		}
	}
}

// 0: Relay_Command parameter
// 1: Command Value
typedef map<string, RelayCommand> RelayCommandHashMap

// 0: Source Type
// 1: Conversion Type
class TypeConversionHashMap
{
	protected ref map<typename, ref TypeConverter> m_Values = new map<typename, ref TypeConverter>();
		
	void ~TypeConversionHashMap()
	{
		delete m_Values;
	}
	
	TypeConverter Get(typename conversion_type)
	{
		TypeConverter result = m_Values.Get(conversion_type);
		
		if (!result) {
			foreach (typename type, TypeConverter conversion: m_Values) {
				if (conversion_type.IsInherited(type)) {
					return conversion;
				}
			}
		}
		
		return result;
	}
	
	void Remove(typename conversion_type) 
	{
		m_Values.Remove(conversion_type);
	}
	
	void Set(typename conversion_type, typename conversion_class) 
	{
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversion_class.ToString()));
			return;
		}
		
		TypeConverter converter = TypeConverter.Cast(conversion_class.Spawn());
		if (!converter) {
			Error(string.Format("TypeConverterHashMap: Failed to create TypeConverter %1", conversion_class));
			return;
		}
		
		m_Values.Set(conversion_type, converter);
	} 
	
	bool Insert(typename conversion_type, typename conversion_class)
	{
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversion_class.ToString()));
			return false;
		}
		
		TypeConverter converter = TypeConverter.Cast(conversion_class.Spawn());
		if (!converter) {
			Error(string.Format("TypeConverterHashMap: Failed to create TypeConverter %1", conversion_class));
			return false;
		}
		
		return m_Values.Insert(conversion_type, converter);
	}
}