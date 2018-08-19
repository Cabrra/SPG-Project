using UnityEngine;
using System.Collections;

public class FloatingTextComponent : MonoBehaviour 
{
	public GUIStyle customLabel;
	
	public Vector2 screenPos = new Vector2(0,0);
	public string theText = "";

	public void Begin(Vector3 worldPosition, float duration, int yDistance, string text, Color color)
	{
		customLabel = new GUIStyle();
		customLabel.alignment = TextAnchor.MiddleCenter;
		customLabel.normal.textColor = color;
		customLabel.fontSize = 32;
		
		screenPos = Camera.main.WorldToScreenPoint(worldPosition);
		screenPos.y = Screen.height - screenPos.y;
		
		theText = text;
		
		StartCoroutine(FloatUp(duration, yDistance));
	}
	
	void OnGUI () 
	{
		GUI.Label(new Rect(screenPos.x - 64, screenPos.y - customLabel.lineHeight/2, 128, customLabel.lineHeight), theText, customLabel);
	}
	
	public IEnumerator FloatUp(float the_time, int yDistance)
	{			
		float originalTime = the_time;
		float pos = screenPos.y;
		float endPos = screenPos.y + yDistance;
		while (the_time > 0.0)
	   {
			the_time -= Time.deltaTime;
			screenPos.y = Mathf.Lerp(endPos, pos, the_time / originalTime);
			Color thisColor = customLabel.normal.textColor;
			thisColor.a -= 0.005f;
			customLabel.normal.textColor = thisColor;
			yield return 0;
	   }
	   
	   Destroy(gameObject);
	} 
}
