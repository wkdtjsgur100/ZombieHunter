#pragma once

//////////////////////  WEAPON /////////////////////////////////////

///샷건
#define SG_MAX_BULLET 12            //초기에 소지하고 있는 최대 총알 개수 
#define SG_RELOADABLE_NUM	3	   //한번에 장전될 수 있는 총알 개수
#define SG_RANGE 400				   //사거리(단위 픽셀)
#define SG_BULLETNUM 5			   //한번 쐈을때 나가는 총알의 갯수
#define SG_COOLTIME	2000		   //사용 쿨타임(단위 밀리세컨드)
#define SG_DAMAGE	2			   //데미지(int)
#define SG_PIERCING	50		   //관통력 (관통력이 1이면 한마리가 맞으면 총알을 제거시킴..)
#define SG_BULLET_SPEED	25	   //총알 날아가는 속도(
#define SG_THETA 50.7f         //총알 날라가는 범위 각도(쏘는 방향을 theta라 하면 -SG_THETA/2<theta<SG_THETA 방향으로 날아간다)

///권총
#define P_MAX_BULLET  100           //초기에 소지하고 있는 최대 총알 개수 
#define P_RELOADABLE_NUM 8		   //한번에 장전될 수 있는 총알 개수
#define P_RANGE	250			   //사거리(단위 픽셀)
#define P_COOLTIME	100 		   //사용 쿨타임(단위 밀리세컨드)
#define P_DAMAGE	1			   //데미지(int)
#define P_HEADSHOT_DAMAGE 3       //헤드샷 데미지
#define P_PIERCING	1		   //관통력
#define P_BULLET_SPEED  25		   //총알속도.

///따발총
#define DD_MAX_BULLET  120           //초기에 소지하고 있는 최대 총알 개수 
#define DD_RELOADABLE_NUM 30		   //한번에 장전될 수 있는 총알 개수
#define DD_RANGE	2000			   //사거리(단위 픽셀)
#define DD_COOLTIME	100		   //사용 쿨타임(단위 밀리세컨드)
#define DD_DAMAGE	1			   //데미지(int)
#define DD_PIERCING	3		   //관통력
#define DD_BULLET_SPEED	30	   //총알속도.

///파이어건
#define FG_MAX_BULLET  70            //초기에 소지하고 있는 최대 총알 개수 
#define FG_RELOADABLE_NUM	70	   //한번에 장전될 수 있는 총알 개수
#define FG_RANGE	300			   //사거리(단위 픽셀)
#define FG_COOLTIME	10		   //사용 쿨타임(단위 밀리세컨드)
#define FG_DAMAGE	1			   //데미지(int)
#define FG_PIERCING	100		   //관통력
#define FG_BULLET_SPEED	25	   //총알속도.
#define FG_BULLET_NUM	1	   //한번 쐈을 때 나가는 총알의 갯수
#define FG_THETA	10.0f			//불길의 범위 각도.(샷건과 동일)

////저격총
#define RIFLE_MAX_BULLET 12            //초기에 소지하고 있는 최대 총알 개수 
#define RIFLE_RELOADABLE_NUM 3		   //한번에 장전될 수 있는 총알 개수
#define RIFLE_RANGE	2000			   //사거리(단위 픽셀)
#define RIFLE_COOLTIME	2000		   //사용 쿨타임(단위 밀리세컨드)
#define RIFLE_DAMAGE	1			   //데미지
#define RIFLE_HEADSHOT_DAMAGE  10      //헤드샷 데미지
#define RIFLE_PIERCING	500		   //관통력
#define RIFLE_BULLET_SPEED	25	   //총알속도.

//////////////////////// ZOMBIE /////////////////////////////

