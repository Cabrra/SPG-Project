using UnityEngine;
using System.Collections;

public class PauseKeyboardControls : MonoBehaviour {

	// Use this for initialization
	void Start () {
		guiTexture.enabled = false;
	}
	
	// Update is called once per frame
	void Update () {
	
		if (PauseScreen.Instance.keyboardCont == true)
			guiTexture.enabled = true;
		else
			guiTexture.enabled = false;
	}
}
