using UnityEngine;
using System.Collections;

public class KeyboardInstrucc : MonoBehaviour {

	public bool isActive = false;
	// Use this for initialization
	void Start () {
		guiTexture.enabled = isActive;
	}
	
	// Update is called once per frame
	void Update () {
	if (Input.GetKeyDown (KeyCode.K))
			{
				isActive = !isActive;
				guiTexture.enabled = isActive;
			}
	}
}
