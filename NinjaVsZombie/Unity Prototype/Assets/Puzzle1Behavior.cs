using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class Puzzle1Behavior : MonoBehaviour 
{

	public bool completed;
	public RoomTrigger doorToActivate;
	public RoomTrigger doorToActivate2;
	//int num;

	public GameObject lever1;
	public GameObject lever2;
	public GameObject lever3;
	public GameObject lever4;
	public GameObject lever5;

	List<GameObject> arr;
	List<GameObject> order;

	// Use this for initialization
	void Start () 
	{
		arr = new List<GameObject>();
		order = new List<GameObject>();

		arr.Add(lever4);
		arr.Add(lever3);
		arr.Add(lever5);
		arr.Add(lever2);
		arr.Add(lever1);

		//num = 0;
	}
	
	// Update is called once per frame
	public void checkPuzzle (GameObject newLever) 
	{
		if(completed == false && newLever.tag != "Lever1" && newLever.tag != "Lever2")
		{
			order.Add(newLever);

			for(int x = 0; x < order.Count; x++)
			{
				if(order[x].GetInstanceID() == arr[x].GetInstanceID())
				{
					doorToActivate.unlocked = false;
					doorToActivate2.unlocked = false;

					if(x >= 4)
					{
						completed = true;
						doorToActivate.unlocked = true;
						doorToActivate2.unlocked = true;
						break;
					}
				}
				else
				{
					reset();
					break;
				}
			}
		}
	}

	void reset()
	{

		//num = 0;
		for (int i = 0; i < arr.Count; i++)
				((GameObject)arr [i]).GetComponent<LeverBehavior>().isOn = false;
        
		order.Clear ();
                  
   
	}
//		print("chhecj");
//		if(!completed)
//		{
//			int x = num;
//			for(;x < 5;x++)
//			{
//				int v = vals[num];
//				if(arr[x].GetComponent<LeverBehavior>().isOn == true)
//					order.Add(arr[x]);
//
//				if(order.Count > 0)
//				{
//					if((x+1) == v)
//					{
//						num++;
//						break;
//					}
//					else
//					{
//						num = 0;
//						for(int i = 0;i < arr.Count;i++)
//							((GameObject)arr[i]).GetComponent<LeverBehavior>().isOn = false;
//					
//						order.Clear();
//					
//						break;
//					}
//				}
//
//				if(num >= 5)
//				{
//					completed = true;
//					num = 5;
//					doorToActivate.unlocked = true;
//				}
//				else
//				{
//					completed = false;
//					doorToActivate.unlocked = false;
//				}
//			}
//		}	}
}
