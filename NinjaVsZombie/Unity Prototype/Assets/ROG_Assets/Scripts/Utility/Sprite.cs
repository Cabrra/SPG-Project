using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Sprite : MonoBehaviour 
{
	public float framesPerSecond = 10;
	public int animationDirection = 1;
	//public int idleIndex = 0;
	public int facing = 1;
	//bool playUntilComplete = false;
	public Texture2D[] sprites;
	public int[] defaultAnimation;
	public int[] moveAnimation;
	public int[] attackAnimation;
	public int[] jumpAnimation;
	
	List<int[]> animationList;
	
	public enum AnimationType {DEFAULT, MOVE, ATTACK, JUMP};
	public AnimationType currentAnimation = AnimationType.DEFAULT;
	
	int animationElement = 0;
	float animationTimer = 0;
	//Vector3 lastPosition = Vector3.zero;
	
	void Start()
	{
		renderer.material.mainTextureScale =  new Vector2(1,facing);
		//lastPosition = transform.position;
		
		animationList = new List<int[]>() {defaultAnimation, moveAnimation, attackAnimation, jumpAnimation};		
	}
	
	void Update()
	{
				
//		if(Input.GetMouseButton(0))
//			ChangeAnimation(AnimationType.ATTACK);
//		// if falling/jumping
//		else if(lastPosition.y != transform.position.y)
//			ChangeAnimation(AnimationType.JUMP);
//		// if moving
//		else if(lastPosition != transform.position)	
//			ChangeAnimation(AnimationType.MOVE);								
//		// default
//		else			
//			ChangeAnimation(AnimationType.DEFAULT);

//		Animate();
	}
	
	public void Animate()
	{
		//AnimationType newAnimation = currentAnimation;
		
		animationElement = Mathf.Abs((int)(animationTimer * framesPerSecond * animationDirection));
		animationElement = animationElement % (animationList[(int)currentAnimation].Length);
//		if(playUntilComplete && animationElement == animationList[(int)currentAnimation].Length)
//			playUntilComplete = false;
		
		renderer.material.mainTexture = sprites[animationList[(int)currentAnimation][animationElement]];
		//lastPosition = transform.position;
		animationTimer += Time.deltaTime;
	}
	
	public void ChangeAnimation(AnimationType newType)
	{
		ChangeAnimation(newType, false);
	}
	
	public void ChangeAnimation(AnimationType newType, bool forcePlayUntilComplete)
	{				
		if(currentAnimation != newType)
		{
			currentAnimation = newType;
			animationElement = 0;
			animationTimer = 0;
		}
		
//		playUntilComplete = forcePlayUntilComplete;
	}
}
