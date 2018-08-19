using UnityEngine;
using System.Collections;

public class Instructions : MonoBehaviour {
	
	public Texture2D instructionsTex;

	void OnGUI()
	{
		if(instructionsTex == null)
			return;
		
		GUI.DrawTexture(new Rect(0,0, Screen.width, Screen.height), instructionsTex);
		
		Vector2 size = new Vector2(Screen.width * 0.25f, Screen.height * 0.1f);
		
		if(GUI.Button(new Rect(Screen.width/2 - size.x/2, Screen.height * 0.85f, size.x, size.y), "Play"))
		{
			Application.LoadLevel("Example_Sandbox");
		}
	}
}
