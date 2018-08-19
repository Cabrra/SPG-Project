using UnityEngine;
using System.Collections;

public class ShowLogo : MonoBehaviour {

	float timer1 = 5.0f;
	float timer2 = 3.5f;
	public float fadeSpeed = 1.5f; 
	bool end = false;

	void Start () {
		guiTexture.enabled = false;
		audio.PlayDelayed(5.5f);
	}
	
	// Update is called once per frame
	void Update () 
	{

		if (timer1 > 0)
				timer1 -= Time.deltaTime;		

		if (timer1 < 0) 
		{
//			if (sound == false)
//			{
//				sound = true;
//				audio.PlayDelayed(3.7f);
//			}
			guiTexture.enabled = true;
			timer2 -= Time.deltaTime;
			if (timer2 < 0)
				EndScene();
		}
		if (end == true)
			LoadLevel ();
	}

	
	void FadeToClear ()
	{
		// Lerp the colour of the texture between itself and transparent.
		guiTexture.color = Color.Lerp(guiTexture.color, Color.clear, fadeSpeed * Time.deltaTime);
	}
	

	void EndScene ()
	{
		FadeToClear ();
		//almost visible
		if(guiTexture.color.a <= 0.05f)
		{
			// ... set the colour to clear and disable the GUITexture.
			guiTexture.color = Color.clear;
			guiTexture.enabled = false;
			end = true;
		}
	}

	void LoadLevel()
	{
		Application.LoadLevel (1);
	}
}