using UnityEngine;
using System.Collections;

public class MiniMapCamera : MonoBehaviour {

	// Use this for initialization
	void Start () 
	{
		camera.enabled = false;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (PauseScreen.Instance.miniMap == true)
			camera.enabled = true;
		else
		    camera.enabled = false;
	}
}
