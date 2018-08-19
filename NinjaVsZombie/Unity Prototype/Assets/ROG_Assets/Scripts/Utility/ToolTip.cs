using UnityEngine;
using System.Collections;

public class ToolTip : MonoBehaviour 
{	
	public		GUIContent		toolTip;
	private		GUIStyle		style;
	
	private		bool 			mouseOver = false;
	private		Rect			drawRect;
	
	
	void Start()
	{
		style = new GUIStyle();
		style.fontSize = 14;
		style.normal.textColor = Color.yellow;
		style.alignment = TextAnchor.MiddleCenter;
	}
	
	
	void OnMouseEnter()
	{
		mouseOver = true;
	}
	
	
	void OnMouseExit()
	{
		mouseOver = false;
	}
	
	
	void Update()
	{					
		if(!mouseOver)
			return;

		Vector2 contentSize = new Vector2(Screen.width/5, Screen.height/5); //style.CalcSize(toolTip);
		
		Vector3 screenPos = Input.mousePosition; //Camera.main.WorldToScreenPoint(transform.position);
		screenPos.y = Screen.height - screenPos.y;		
         
		drawRect = new Rect(screenPos.x - contentSize.x/2, screenPos.y - contentSize.y/2, contentSize.x, contentSize.y);
	}
	
	
	void OnGUI()
	{
		if(!mouseOver)
			return;
		
		//GUI.Box(drawRect,"");
		ROG.DrawBox(drawRect, Color.black, 0.6f);
		GUI.Label(drawRect, toolTip, style);
	}
}
