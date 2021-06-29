class DialogCategoryBaseController: DialogBaseController
{
	ref ObservableCollection<ref DialogCategoryListItem> DialogCategoryData = new ObservableCollection<ref DialogCategoryListItem>(this);		
		
	void ~DialogCategoryBaseController()
	{
		delete DialogCategoryData;
	}
}

class DialogCategoryBase: DialogBase
{
	protected DialogCategoryBaseController m_DialogCategoryBaseController;
	
	void DialogCategoryBase(string title)
	{
		m_DialogCategoryBaseController = DialogCategoryBaseController.Cast(m_Controller);
	}
	
	void AddContent(string caption, array<ref ScriptView> content)
	{
		DialogCategoryListItem category_list_item = new DialogCategoryListItem(caption);
		foreach (ScriptView script_view: content) {
			category_list_item.AddContent(script_view);
			script_view.SetParent(this);
			script_view.GetLayoutRoot().Show(false);
			m_DialogCategoryBaseController.DialogContentData.Insert(script_view);
		}
		
		category_list_item.SetParent(this);
		m_DialogCategoryBaseController.DialogCategoryData.Insert(category_list_item);
	}
	
	void AddContent(DialogCategoryListItem category_list_item)
	{
		array<ref ScriptView> content = category_list_item.GetContent();
		foreach (ScriptView script_view: content) {
			script_view.SetParent(this);
			script_view.GetLayoutRoot().Show(false);
			m_DialogCategoryBaseController.DialogContentData.Insert(script_view);
		}
		
		category_list_item.SetParent(this);
		
		m_DialogCategoryBaseController.DialogCategoryData.Insert(category_list_item);
	}
	
	void SetActiveCategory(DialogCategoryListItem list_item)
	{
		array<ref ScriptView> set_content = list_item.GetContent();
		for (int i = 0; i < m_DialogCategoryBaseController.DialogContentData.Count(); i++) {
			m_DialogCategoryBaseController.DialogContentData[i].GetLayoutRoot().Show(set_content.Find(m_DialogCategoryBaseController.DialogContentData[i]) != -1);
		}
	}
	
	override typename GetControllerType() 
	{
		return DialogCategoryBaseController;
	}	
	
	override string GetLayoutFile()
	{
		return "DabsFramework/GUI/layouts/dialogs/DialogCategoryBase.layout";
	}
}

class DialogCategoryListItemController: ViewController
{
	string Icon;
	string Caption;
}

class DialogCategoryListItem: ScriptViewTemplate<DialogCategoryListItemController>
{
	protected DialogCategoryBase m_DialogCategoryBase;
	
	protected ref array<ref ScriptView> m_Content = {};
	
	void DialogCategoryListItem(string caption)
	{
		m_TemplateController.Caption = caption;
		m_TemplateController.NotifyPropertyChanged("Caption");
	}
		
	void OnListItemExecute(ButtonCommandArgs args)
	{
		if (!m_DialogCategoryBase) {
			return;
		}
		
		m_DialogCategoryBase.SetActiveCategory(this);
	}
	
	void SetParent(DialogCategoryBase dialog)
	{
		m_DialogCategoryBase = dialog;
	}
	
	void AddContent(ScriptView content)
	{
		m_Content.Insert(content);
	}
	
	array<ref ScriptView> GetContent()
	{
		return m_Content;
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/ListItems/CategorizedListItem.layout";
	}
}