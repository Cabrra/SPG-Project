// Rules of the Game function library
// Date:	9-2-2013

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public static class ROG
{		
	static Texture2D bitmapTexture;
	static Texture2D circleTexture;

	
	// Check Line of Sight, returns true if there is no collision detected between the start position and the target object
	public static bool hasLOS(Vector3 startPos, float range, GameObject targetObj)
	{
		if(!targetObj)
			return false;
		
		Vector3 dir = (targetObj.transform.position - startPos).normalized;
		
		//Debug.DrawLine(startPos, targetObj.transform.position);
		
		RaycastHit hit;
		
		if(Physics.Raycast(startPos, dir, out hit, range))
		{			
			if(hit.collider.gameObject.GetInstanceID() == targetObj.GetInstanceID())						
				return true;
		}

		return false;
	}
	
	// Check Line of Sight, returns true if there is no collision detected between the start and end position
	public static bool hasLOS(Vector3 startPos, Vector3 endPos)
	{		
		float distance = Vector3.Distance(startPos, endPos);
		Vector3 direction = (endPos - startPos).normalized;
		
		//Debug.DrawLine(startPos, endPos);

		if(!Physics.Raycast(startPos, direction, distance))					
				return true;
		
		return false;
	}
	
	// Check Line of Sight, returns true if there is no collision detected between the start and end position
	public static bool hasLOS(Vector3 startPos, Vector3 endPos, LayerMask layer)
	{		
		float distance = Vector3.Distance(startPos, endPos);
		Vector3 direction = (endPos - startPos).normalized;
		
		//Debug.DrawLine(startPos, endPos);

		if(!Physics.Raycast(startPos, direction, distance, layer))					
				return true;
		
		return false;
	}	
	
	// Check Line of Sight, returns true if there is no collision detected between the start position and the target object
	public static bool hasLOS(Vector3 startPos, float range, GameObject targetObj, LayerMask layer)
	{
		if(!targetObj)
			return false;
		
		Vector3 dir = (targetObj.transform.position - startPos).normalized;
		
		//Debug.DrawLine(startPos, targetObj.transform.position);
		
		RaycastHit hit;
		
		if(Physics.Raycast(startPos, dir, out hit, range, layer))
		{			
			if(hit.collider.gameObject.GetInstanceID() == targetObj.GetInstanceID())						
				return true;
		}

		return false;
	}
	
	public static bool hasLOS(Vector3 startPos, float range, GameObject targetObj, LayerMask layer, bool ignoreLayer)
	{
		if(ignoreLayer)
			return hasLOS(startPos, range, targetObj, ~layer);
		else
			return hasLOS(startPos, range, targetObj, layer);
	}

	// Returns the Vector3 position of the mouse in 3D space
	public static Vector3 GetMouseWorldPosition(GameObject fromObject)
	{
		// Assumes use of default "Main Camera"
		float camDist = Vector3.Distance(fromObject.transform.position, Camera.main.transform.position);
		Vector3 mousePos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, camDist));		
		return mousePos;
	}	
	
	// Returns the Vector3 position of the mouse in 3D space
	public static Vector3 GetMouseWorldPosition(Camera fromCamera, GameObject fromObject)
	{
		// Specify camera and from Object
		float camDist = Vector3.Distance(fromObject.transform.position, fromCamera.transform.position);
		Vector3 mousePos = fromCamera.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, camDist));
		return mousePos;
	}	
	
	// Returns the Vector3 direction from the specified object's position to the mouse 3D position
	public static Vector3 GetDirectionToMouse(GameObject fromObject)		
	{
		// Assumes use of default "Main Camera"
		Vector3 mPos = GetMouseWorldPosition(fromObject);
		Vector3 mouseDirection = (mPos - fromObject.transform.position).normalized;
		return mouseDirection;
	}
	
	// Returns the Vector3 direction from the specified object's position to the mouse 3D position
	// Optional bool to override height, use this for most Top-Down shooters
	public static Vector3 GetDirectionToMouse(GameObject fromObject, bool ignoreUp)		
	{
		// Assumes use of default "Main Camera"
		Vector3 mPos = GetMouseWorldPosition(fromObject);
		
		if(ignoreUp)
			mPos.y = fromObject.transform.position.y;
		
		Vector3 mouseDirection = (mPos - fromObject.transform.position).normalized;
		return mouseDirection;
	}
	
	// Returns the Vector3 direction from the specified object's position to the mouse 3D position
	public static Vector3 GetDirectionToMouse(Camera fromCamera, GameObject fromObject)		
	{
		// Specify camera and from Object
		Vector3 mPos = GetMouseWorldPosition(fromCamera, fromObject);
		Vector3 mouseDirection = (mPos - fromObject.transform.position).normalized;
		return mouseDirection;
	}		
		
	public static Quaternion LookAtMouse(GameObject obj, bool ignoreUp)
	{
		Vector3 lookVector = GetDirectionToMouse(obj, ignoreUp);
		
		if(lookVector.magnitude != 0)
			return Quaternion.LookRotation(lookVector);
		else
			return Quaternion.identity;
	}
	
//	public static Quaternion GetRotationToMouse(GameObject obj, bool ignoreUp)
//	{
//		Vector3 lookVector = GetDirectionToMouse(obj, ignoreUp);
//		
//		if(lookVector.magnitude != 0)
//			return Quaternion.LookRotation(lookVector);
//		else
//			return Quaternion.identity;
//	}
	
	
	// GetSpreadShotRotations: Returns an array of Quaternions representing the direction of each spread shot
	public static Quaternion[] GetSpreadShotRotations(int numShots, float degreeOffset, Vector3 facing, Vector3 rotationAxis)
	{
		Quaternion[] directions = new Quaternion[numShots];
		
		for(int i = 0; i < numShots; i++)
		{			
			directions[i] = Quaternion.LookRotation(Quaternion.AngleAxis(degreeOffset * i - degreeOffset * (numShots-1) * 0.5f, rotationAxis) * facing);
		}
		
		return directions;		
	}
	
	
	// GetSpreadShotVectors: Returns an array of Vector3 representing the direction of each spread shot
	public static Vector3[] GetSpreadShotVectors(int numShots, float degreeOffset, Vector3 facing, Vector3 rotationAxis)
	{
		Vector3[] directions = new Vector3[numShots];
		
		for(int i = 0; i < numShots; i++)
		{			
			directions[i] = Quaternion.AngleAxis(degreeOffset * i - degreeOffset * (numShots-1) * 0.5f, rotationAxis) * facing;
		}
		
		return directions;		
	}
	
	
	// Spread Shot
	public static GameObject[] SpreadShot(GameObject projectile, int numShots, float degreeOffset, Vector3 position, float positionOffset, Vector3 facing, Vector3 rotationAxis)
	{			
		GameObject[] bulletInstances = new GameObject[numShots];
		Quaternion[] shootDirections = ROG.GetSpreadShotRotations(numShots, degreeOffset, facing, rotationAxis);
		
		for(int j = 0; j < shootDirections.Length; j++)
		{			
			Vector3 shootPosition = (position + shootDirections[j] * facing * positionOffset);
			GameObject bulletObject = (GameObject)MonoBehaviour.Instantiate(projectile, shootPosition, shootDirections[j]);
			bulletInstances[j] = bulletObject;
		}
		
		return bulletInstances;
	}
	
	
	// Finding by Tag -----------------------------------------------	
	
	// Find the closest object (by tag) to the given Vector3 position
	public static GameObject FindNearestObjectByTag(Vector3 fromPosition, string tag) 
	{
		GameObject[] gos = GameObject.FindGameObjectsWithTag(tag);
		
		if(gos.Length == 1)
			return gos[0];
		
		GameObject closest = null;
		float distance = Mathf.Infinity;
		Vector3 position = fromPosition;
		
		foreach (GameObject go in gos) 
		{
			Vector3 diff = go.transform.position - position;
			float curDistance = diff.sqrMagnitude;
			if (curDistance < distance) 
			{
				closest = go;
				distance = curDistance;
			}
		}
		
		return closest;
	}
	
	// Find the "Next" closest object while ignoring the object passed in by ID, use gameObject.GetInstanceID()
	public static GameObject FindNextNearestObjectByTag(Vector3 fromPosition, string tag, int ignoreID) 
	{
		GameObject[] gos = GameObject.FindGameObjectsWithTag(tag);
		
		if(gos.Length == 1)
			return gos[0];
		
		GameObject closest = null;
		float distance = Mathf.Infinity;
		Vector3 position = fromPosition;
		
		foreach (GameObject go in gos) 
		{
			Vector3 diff = go.transform.position - position;
			float curDistance = diff.sqrMagnitude;
			if (curDistance < distance && go.GetInstanceID() != ignoreID) 
			{
				closest = go;
				distance = curDistance;
			}
		}
		
		return closest;
	}
	
	// Find the "Next" closest object while ignoring the object passed in
	public static GameObject FindNextNearestObjectByTag(Vector3 fromPosition, string tag, GameObject ignoreObj)	
	{
		return FindNextNearestObjectByTag(fromPosition, tag, ignoreObj.GetInstanceID());				
	}
	
	// Overload that takes in multiple IDs to ignore
	public static GameObject FindNextNearestObjectByTag(Vector3 fromPosition, string tag, int[] ignoreIDs) 
	{
		GameObject[] gos = GameObject.FindGameObjectsWithTag(tag);
		
		if(gos.Length == 1)
			return gos[0];
		
		GameObject closest = null;
		float distance = Mathf.Infinity;
		Vector3 position = fromPosition;
		
		foreach (GameObject go in gos) 
		{
			Vector3 diff = go.transform.position - position;
			float curDistance = diff.sqrMagnitude;			
			bool ignoreCurrentObj = false;
			
			foreach(int i in ignoreIDs)
			{
				if(go.GetInstanceID() == i)
				{
					ignoreCurrentObj = true;
					break;
				}
			}
			
			if (!ignoreCurrentObj && curDistance < distance)
			{
				closest = go;
				distance = curDistance;
			}
		}
		
		return closest;
	}	

	// Overload that takes in multiple IDs to ignore
	public static GameObject FindNextNearestObjectByTag(Vector3 fromPosition, string tag, List<int> ignoreIDs) 
	{
		int[] ignores = ignoreIDs.ToArray();		
		return FindNextNearestObjectByTag(fromPosition, tag, ignores);		
	}	
	
	// Overload that takes in multiple objects to ignore
	public static GameObject FindNextNearestObjectByTag(Vector3 fromPosition, string tag, GameObject[] ignoreObjs) 
	{
		int[] ignores = new int[ignoreObjs.Length];
		for(int i = 0; i < ignoreObjs.Length; i++)
			if(ignoreObjs[i] != null)
				ignores[i] = ignoreObjs[i].GetInstanceID();
		
		return FindNextNearestObjectByTag(fromPosition, tag, ignores);		
	}
	
	// Overload that takes in multiple objects to ignore
	public static GameObject FindNextNearestObjectByTag(Vector3 fromPosition, string tag, List<GameObject> ignoreObjs)
	{
		GameObject[] ignores = ignoreObjs.ToArray();
		return FindNextNearestObjectByTag(fromPosition, tag, ignores);
	}
	
		
	// Finding by Name -----------------------------------------------	
		
	// Find the closest object (by name) to the given Vector3 position
	public static GameObject FindNearestObjectByName(Vector3 fromPosition, string name) 
	{	
		GameObject[] gos = (GameObject[])GameObject.FindObjectsOfType(typeof(GameObject));
		
		if(gos.Length == 1)
			return gos[0];
		
		GameObject closest = null;
		float distance = Mathf.Infinity;
		Vector3 position = fromPosition;
		
		foreach (GameObject go in gos) 
		{
			if(go.name != name)
				continue;
			
			Vector3 diff = go.transform.position - position;
			float curDistance = diff.sqrMagnitude;
			if (curDistance < distance) 
			{
				closest = go;
				distance = curDistance;
			}
		}
		
		return closest;
	}
	
	// Find the "Next" closest object while ignoring the object passed in by ID, use gameObject.GetInstanceID()
	public static GameObject FindNextNearestObjectByName(Vector3 fromPosition, string name, int ignoreID) 
	{
		GameObject[] gos = (GameObject[])GameObject.FindObjectsOfType(typeof(GameObject));		
		
		if(gos.Length == 1)
			return gos[0];
		
		GameObject closest = null;
		float distance = Mathf.Infinity;
		Vector3 position = fromPosition;
		
		foreach (GameObject go in gos) 
		{
			if(go.name != name)
				continue;
			
			Vector3 diff = go.transform.position - position;
			float curDistance = diff.sqrMagnitude;
			if (curDistance < distance && go.GetInstanceID() != ignoreID) 
			{
				closest = go;
				distance = curDistance;
			}
		}
		
		return closest;
	}
	
	// Find the "Next" closest object while ignoring the object passed in
	public static GameObject FindNextNearestObjectByName(Vector3 fromPosition, string name, GameObject ignoreObj)
	{		
		return FindNextNearestObjectByName(fromPosition, name, ignoreObj.GetInstanceID());		
	}
	
	// Overload that takes in multiple IDs to ignore
	public static GameObject FindNextNearestObjectByName(Vector3 fromPosition, string name, int[] ignoreIDs) 
	{
		GameObject[] gos = (GameObject[])GameObject.FindObjectsOfType(typeof(GameObject));
		
		if(gos.Length == 1)
			return gos[0];
		
		GameObject closest = null;
		float distance = Mathf.Infinity;
		Vector3 position = fromPosition;
		
		foreach (GameObject go in gos) 
		{
			if(go.name != name)
				continue;
			
			Vector3 diff = go.transform.position - position;
			float curDistance = diff.sqrMagnitude;
			bool ignoreCurrentObj = false;
			
			foreach(int i in ignoreIDs)
			{
				if(go.GetInstanceID() == i)
				{
					ignoreCurrentObj = true;
					break;
				}
			}
			
			if (!ignoreCurrentObj && curDistance < distance)
			{
				closest = go;
				distance = curDistance;
			}
		}
		
		return closest;
	}
		
	// Overload that takes in multiple IDs to ignore
	public static GameObject FindNextNearestObjectByName(Vector3 fromPosition, string name, List<int> ignoreIDs) 
	{
		int[] ignores = ignoreIDs.ToArray();		
		return FindNextNearestObjectByName(fromPosition, name, ignores);
	}
	
	// Overload that takes in multiple objects to ignore
	public static GameObject FindNextNearestObjectByName(Vector3 fromPosition, string name, GameObject[] ignoreObjs) 
	{
		int[] ignores = new int[ignoreObjs.Length];
		for(int i = 0; i < ignoreObjs.Length; i++)
			if(ignoreObjs[i] != null)
				ignores[i] = ignoreObjs[i].GetInstanceID();
		
		return FindNextNearestObjectByName(fromPosition, name, ignores);		
	}
	
	// Overload that takes in multiple objects to ignore	
	public static GameObject FindNextNearestObjectByName(Vector3 fromPosition, string name, List<GameObject> ignoreObjs)
	{
		GameObject[] ignores = ignoreObjs.ToArray();
		return FindNextNearestObjectByName(fromPosition, name, ignores);
	}
	
	
	// Playing Sounds -----------------------------------------------	
	
	// Plays a sound in 3D space (Make sure the sound is marked as 3D in the inspector)
    public static AudioSource PlaySoundIn3D(AudioClip clip, Vector3 position, float volume) 
	{
        GameObject go = new GameObject("One-shot audio 3D");
        go.transform.position = position;
        AudioSource source = go.AddComponent<AudioSource>();		
        source.clip = clip;
        source.volume = volume;
        source.Play();
        GameObject.Destroy(go, clip.length);
        return source;
    }
	
	
	// Plays a sound normal (Make sure the sound is marked as 2D in the inspector)
	public static AudioSource PlaySound(AudioClip clip)
	{
        GameObject go = new GameObject("One-shot audio 2D");
        go.transform.position = Camera.main.transform.position;
        AudioSource source = go.AddComponent<AudioSource>();
        source.clip = clip;
        source.volume = 1.0f;
        source.Play();
        GameObject.Destroy(go, clip.length);
        return source;
    }
	
	// Plays a sound at volume
	public static AudioSource PlaySound(AudioClip clip, float volume)
	{
        GameObject go = new GameObject("One-shot audio");
        go.transform.position = Camera.main.transform.position;
        AudioSource source = go.AddComponent<AudioSource>();
        source.clip = clip;
        source.volume = volume;
        source.Play();
        GameObject.Destroy(go, clip.length);
        return source;
    }
	
	//--------------------Progress Bars--------------------
	
	// Draws a progress bar, various overloads
	public static void DrawHealthBar(float percent, int x, int y, int w, int h, GUIStyle style)
	{
		DrawBar(percent, x, y, w, h, Color.red, "Health", style);
	}
	public static void DrawManaBar(float percent, int x, int y, int w, int h, GUIStyle style)
	{
		DrawBar(percent, x, y, w, h, Color.blue, "Mana", style);
	}
	public static void DrawBar(float percent, int x, int y, int width, int height, Color fillColor, string label, GUIStyle style)
	{				
		if(bitmapTexture == null)
			bitmapTexture = (Texture2D)Resources.Load("WhitePixel");
			
		// Temporarily change GUI color (to Black with 40% alpha)
		Color previousColor = GUI.color;
		GUI.color = new Color(0,0,0,0.5f);
		
		// Draw Bar Background texture
		GUI.DrawTexture(new Rect(x, y, width, height), bitmapTexture);
		
		// Temporarily change GUI color (at 75% alpha)
		GUI.color = new Color(fillColor.r, fillColor.g, fillColor.b, 0.75f);
		
		// Draw Bar Fill texture
		GUI.DrawTexture(new Rect(x, y, width * percent, height), bitmapTexture);
						
		// Change GUI color back to normal
		GUI.color = previousColor;
		
		// Center text alignment for labels 
		GUI.skin.label.alignment = TextAnchor.MiddleCenter;
		
		// Draw Text Label
		GUI.Label(new Rect(x, y, width, height), label, style);
	}
	
	
	public static void DrawBox(float x, float y, float width, float height, Color fillColor, float alpha)
	{				
		if(bitmapTexture == null)
			bitmapTexture = (Texture2D)Resources.Load("WhitePixel");

		Color previousColor = GUI.color;
		GUI.color = new Color(fillColor.r, fillColor.g, fillColor.b, alpha);
		
		// Draw Bar Background texture
		GUI.DrawTexture(new Rect(x, y, width, height), bitmapTexture);

		GUI.color = previousColor;
	}
		
	
	public static void DrawBox(Rect r, Color fillColor, float alpha)
	{				
		DrawBox(r.x, r.y, r.width, r.height, fillColor, alpha);
	}
	
	
	public static void DrawBoxOutline(Rect r, int thickness, Color color, float alpha)
	{	
		// top
		DrawBox(r.x, r.y, r.width, thickness, color, alpha);
		// right
		DrawBox(r.x + r.width, r.y, thickness, r.height, color, alpha);
		// bottom				
		DrawBox(r.x + thickness, r.y + r.height, r.width, thickness, color, alpha);
		// left
		DrawBox(r.x, r.y + thickness, thickness, r.height, color, alpha);
	}
	
	// Draw Healthbar at world position (converts from world to screen space)	
	public static void DrawHealthBarAtWorldPosition(float percent, Vector3 worldPosition, int width, int height, int verticalOffset, Color color, float alpha, GUIStyle style)
	{	
		Vector3 screenPosition = Camera.main.WorldToScreenPoint(worldPosition);
		screenPosition.y = Screen.height - screenPosition.y;
		
		DrawBar(percent, (int)screenPosition.x - width/2, (int)screenPosition.y - height/2 + verticalOffset, width, height, color, "", style);
	}
	
	
	public static void DrawCircle(float centerX, float centerY, float radius, Color color, float alpha)
	{		
		if(circleTexture == null)
			circleTexture = (Texture2D)Resources.Load("WhiteCircle_256x256");
		
		//float size = radius/128.0f;
		//MonoBehaviour.print(size);
		Rect r = new Rect(centerX - radius, centerY - radius, radius * 2, radius * 2);
		
		Color previousColor = GUI.color;
		GUI.color = new Color(color.r, color.g, color.b, alpha);
		
		GUI.DrawTexture(r, circleTexture);
		
		GUI.color = previousColor;
	}
	
	
	//--------------------AOE Damage-----------------------
	
	// Calls the public function TakeDamage() on all objects in the radius, and passes the damage value
	public static void AOE_Damage(Vector3 position, float damage, float radius)
	{
		Collider[] cols = Physics.OverlapSphere(position, radius);
		foreach(Collider col in cols)			
			col.gameObject.BroadcastMessage("ModifyHealth", -damage, SendMessageOptions.DontRequireReceiver);
	}
	
	// overload that includes an object to ignore (usually the player)
	public static void AOE_Damage(Vector3 position, float damage, float radius, GameObject ignore)
	{
		Collider[] cols = Physics.OverlapSphere(position, radius);
		foreach(Collider col in cols)		
			if(col.gameObject.GetInstanceID() != ignore.GetInstanceID())
				col.gameObject.BroadcastMessage("ModifyHealth", -damage, SendMessageOptions.DontRequireReceiver);
	}
	
	//--------------------AOE Push-------------------------
	
	// Pushes away all rigidbody objects in the radius from the specified position
	public static void AOE_Push(Vector3 position, float amount, float radius)
	{
		Collider[] cols = Physics.OverlapSphere(position, radius);
		foreach(Collider col in cols)			
			if(col.rigidbody)
			{
				Vector3 direction = (col.transform.position - position).normalized;
				col.rigidbody.AddForce(direction * amount);
			}
	}
	
	//--------------------AOE Pull-------------------------
	
	// Pulls in all rigidbody objects in the radius from the specified position
	public static void AOE_Pull(Vector3 position, float amount, float radius)
	{
		Collider[] cols = Physics.OverlapSphere(position, radius);
		foreach(Collider col in cols)			
			if(col.rigidbody)
			{
				Vector3 direction = (position - col.transform.position).normalized;
				col.rigidbody.AddForce(direction * amount);
			}
	}
	
	//--------------------AOE Use The Force----------------
	
	// "Lifts" all rigidbody objects in the radius
	public static void UseTheForce(Vector3 position, float amount, float radius)
	{
		Collider[] cols = Physics.OverlapSphere(position, radius);
		foreach(Collider col in cols)	
			if(col.rigidbody)
				col.rigidbody.AddForce(Vector3.up * amount);
	}
}