using UnityEngine;
using System.Collections;

public class firedisappear : MonoBehaviour 
{

	int count;
	public LeverBehavior lever;
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
			this.gameObject.GetComponent<PulseTrap>().enabled = true;
			count = 1;
		}
		
		if(lever.isOn == false)
		{
			this.gameObject.collider.enabled = false;
			this.gameObject.renderer.enabled = false;
			this.gameObject.GetComponent<PulseTrap>().enabled = false;
			count = 0;
		}
	}
}
