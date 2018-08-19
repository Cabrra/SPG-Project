using UnityEngine;
using System.Collections;

public class floorappear : MonoBehaviour 
{
	public LeverBehavior lever;
	int count;
	// Use this for initialization
	void Start () 
	{
		count = 0;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if(lever.isOn && count == 0)
		{
			this.gameObject.collider.enabled = true;
			this.gameObject.renderer.enabled = true;
			count = 1;
		}

		if(lever.isOn == false)
		{
			this.gameObject.collider.enabled = false;
			this.gameObject.renderer.enabled = false;
			count = 0;
		}
	}
}
