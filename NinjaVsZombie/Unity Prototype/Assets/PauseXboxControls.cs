using UnityEngine;
using System.Collections;

public class PauseXboxControls : MonoBehaviour {

	// Use this for initialization
	void Start () 
	{
		guiTexture.enabled = false;
	}
		
		// Update is called once per frame
	void Update () 
	{
			
		if (PauseScreen.Instance.xboxCont == true)
			guiTexture.enabled = true;
		else
			guiTexture.enabled = false;
	}
}