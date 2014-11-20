/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History
$Log: not supported by cvs2svn $
Revision 1.19  2008/05/08 23:50:44  cignoni
renamed vertex quality smoothing
added face normal smoothing FF (and added a VF to the previous face normal smoothing)

Revision 1.18  2008/05/02 09:43:25  cignoni
Added color smoothing, scale dependent laplacian changed a SD_old into SD fujumori, improved comments.

Revision 1.17  2008/04/18 17:48:29  cignoni
added facenormal smoothing

Revision 1.16  2008/02/07 10:24:51  cignoni
added a missing IsD() check

Revision 1.15  2007/11/05 23:47:20  cignoni
added selection to the pasodoble smoothing

Revision 1.14  2007/03/27 09:40:47  cignoni
Changed use of selected to visited flags. Improved variable namings and comments

Revision 1.13  2006/11/07 15:13:56  zifnab1974
Necessary changes for compilation with gcc 3.4.6. Especially the hash function is a problem

Revision 1.12  2006/11/07 11:28:02  cignoni
Added Quality weighted laplacian smoothing

Revision 1.11  2006/10/19 07:33:03  cignoni
Corrected Laplacian, Added selection to HCSmooth

Revision 1.10  2006/09/25 09:41:41  cignoni
Added new version of pasodoble smoothing

Revision 1.9  2006/02/06 10:45:47  cignoni
Added missing typenames

Revision 1.7  2006/01/24 13:23:22  pietroni
used template types instead of point3f and float inside function calls

Revision 1.6  2005/12/06 17:55:16  pietroni
1 bug corrected

Revision 1.5  2005/12/02 16:24:56  pietroni
corrected 1 bug in Cross Prod Gradient

Revision 1.4  2005/11/23 16:24:44  pietroni
corrected CrossProdGradient( )

Revision 1.3  2005/07/11 13:12:05  cignoni
small gcc-related compiling issues (typenames,ending cr, initialization order)

Revision 1.2  2005/03/16 16:14:12  spinelli
aggiunta funzione PasoDobleSmooth e relative:

- FitMesh
- FaceErrorGrad
- CrossProdGradient
- TriAreaGradient
- NormalSmooth

e le classi:

- PDVertInfo
- PDFaceInfo

necessarie per utilizzare SimpleTempData

Revision 1.1  2004/12/11 14:53:19  ganovelli
first partial porting: compiled gcc,intel and msvc


****************************************************************************/


#ifndef __VCGLIB__SMOOTH
#define __VCGLIB__SMOOTH

#include <wrap/callback.h>
#include <vcg/space/point3.h>
#include <vcg/space/line3.h>
#include <vcg/container/simple_temporary_data.h>
#include <vcg/complex/trimesh/update/normal.h>

namespace vcg
{
namespace tri
{
///
/** \addtogroup trimesh */
/*@{*/
/// Class of static functions to smooth and fair meshes and their attributes.

template <class SmoothMeshType>
class Smooth
{

public:
			typedef SmoothMeshType MeshType;
			typedef typename MeshType::VertexType     VertexType;
			typedef typename MeshType::VertexType::CoordType     CoordType;
			typedef typename MeshType::VertexPointer  VertexPointer;
			typedef typename MeshType::VertexIterator VertexIterator;
			typedef	typename MeshType::ScalarType			ScalarType;
			typedef typename MeshType::FaceType       FaceType;
			typedef typename MeshType::FacePointer    FacePointer;
			typedef typename MeshType::FaceIterator   FaceIterator;
			typedef typename MeshType::FaceContainer  FaceContainer;
      typedef typename vcg::Box3<ScalarType>  Box3Type;
    	typedef typename vcg::face::VFIterator<FaceType> VFLocalIterator;

class ScaleLaplacianInfo
{
public:
	CoordType PntSum;
	ScalarType LenSum;
};

// This is precisely what curvature flow does.
// Curvature flow smoothes the surface by moving along the surface
// normal n with a speed equal to the mean curvature
void VertexCoordLaplacianCurvatureFlow(MeshType &m, int step, ScalarType delta)
{

}

// Another Laplacian smoothing variant,
// here we sum the baricenter of the faces incidents on each vertex weighting them with the angle

static void VertexCoordLaplacianAngleWeighted(MeshType &m, int step, ScalarType delta)
{
	ScaleLaplacianInfo lpz;
	lpz.PntSum=CoordType(0,0,0);
	lpz.LenSum=0;
	SimpleTempData<typename MeshType::VertContainer, ScaleLaplacianInfo > TD(m.vert,lpz);
	FaceIterator fi;
	for(int i=0;i<step;++i)
	{
		VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;
		ScalarType a[3];
		for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
		{
			CoordType  mp=((*fi).V(0)->P() + (*fi).V(1)->P() + (*fi).V(2)->P())/3.0;
			CoordType  e0=((*fi).V(0)->P() - (*fi).V(1)->P()).Normalize();
			CoordType  e1=((*fi).V(1)->P() - (*fi).V(2)->P()).Normalize();
			CoordType  e2=((*fi).V(2)->P() - (*fi).V(0)->P()).Normalize();

			a[0]=AngleN(-e0,e2);
			a[1]=AngleN(-e1,e0);
			a[2]=AngleN(-e2,e1);
			//assert(fabs(M_PI -a[0] -a[1] -a[2])<0.0000001);

			for(int j=0;j<3;++j){
						CoordType dir= (mp-(*fi).V(j)->P()).Normalize();
						TD[(*fi).V(j)].PntSum+=dir*a[j];
						TD[(*fi).V(j)].LenSum+=a[j]; // well, it should be named angleSum
			}
		}
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].LenSum>0 )
				 (*vi).P() = (*vi).P() +  (TD[*vi].PntSum/TD[*vi].LenSum ) * delta;

	}
};

// Scale dependent laplacian smoothing [Fujiwara 95]
// as described in
// Implicit Fairing of Irregular Meshes using Diffusion and Curvature Flow
// Mathieu Desbrun, Mark Meyer, Peter Schroeder, Alan H. Barr
// SIGGRAPH 99
// REQUIREMENTS: Border Flags.
//
// Note the delta parameter is in a absolute unit
// to get stability it should be a small percentage of the shortest edge.

static void VertexCoordScaleDependentLaplacian_Fujiwara(MeshType &m, int step, ScalarType delta)
{
	SimpleTempData<typename MeshType::VertContainer, ScaleLaplacianInfo > TD(m.vert);
	ScaleLaplacianInfo lpz;
	lpz.PntSum=CoordType(0,0,0);
	lpz.LenSum=0;
	FaceIterator fi;
	for(int i=0;i<step;++i)
	{
			VertexIterator vi;
			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				 TD[*vi]=lpz;

			for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j)) {
						CoordType edge= (*fi).V1(j)->P() -(*fi).V(j)->P();
						ScalarType len=Norm(edge);
						edge/=len;
						TD[(*fi).V(j)].PntSum+=edge;
						TD[(*fi).V1(j)].PntSum-=edge;
						TD[(*fi).V(j)].LenSum+=len;
						TD[(*fi).V1(j)].LenSum+=len;
					}

			for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					// se l'edge j e' di bordo si riazzera tutto e si riparte
					if((*fi).IsB(j)) {
							TD[(*fi).V(j)].PntSum=CoordType(0,0,0);
							TD[(*fi).V1(j)].PntSum=CoordType(0,0,0);
							TD[(*fi).V(j)].LenSum=0;
							TD[(*fi).V1(j)].LenSum=0;
					}


			for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
						{
							CoordType edge= (*fi).V1(j)->P() -(*fi).V(j)->P();
							ScalarType len=Norm(edge);
							edge/=len;
							TD[(*fi).V(j)].PntSum+=edge;
							TD[(*fi).V1(j)].PntSum-=edge;
							TD[(*fi).V(j)].LenSum+=len;
							TD[(*fi).V1(j)].LenSum+=len;
						}
  // The fundamental part:
	// We move the new point of a quantity
	//
	//  L(M) = 1/Sum(edgelen) * Sum(Normalized edges)
	//

			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].LenSum>0 )
				 (*vi).P() = (*vi).P() + (TD[*vi].PntSum/TD[*vi].LenSum)*delta;
	}
};


class LaplacianInfo
{
public:
	LaplacianInfo(const CoordType &_p, const int _n):sum(_p),cnt(_n) {}
	LaplacianInfo() {}
	CoordType sum;
	ScalarType cnt;
};

// Classical Laplacian Smoothing. Each vertex can be moved onto the average of the adjacent vertices.
// Can smooth only the selected vertices and weight the smoothing according to the quality
// In the latter case 0 means that the vertex is not moved and 1 means that the vertex is moved onto the computed position.
//
// From the Taubin definition "A signal proc approach to fair surface design"
// We define the discrete Laplacian of a discrete surface signal by weighted averages over the neighborhoods
//          \delta xi  = \Sum wij (xj - xi) ;
// where xj are the adjacent vertices of xi and wij is usually 1/n_adj
//
// This function simply accumulate over a TempData all the positions of the ajacent vertices
//
static void AccumulateLaplacianInfo(MeshType &m, SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > &TD)
{
			FaceIterator fi;
			for(fi=m.face.begin();fi!=m.face.end();++fi)
			{
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if(!(*fi).IsB(j))
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
						}
			}
			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
			{
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
						{
							TD[(*fi).V0(j)].sum=(*fi).P0(j);
							TD[(*fi).V1(j)].sum=(*fi).P1(j);
							TD[(*fi).V0(j)].cnt=1;
							TD[(*fi).V1(j)].cnt=1;
						}
			}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
			for(fi=m.face.begin();fi!=m.face.end();++fi)
			{
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
						}
			}
}

static void VertexCoordLaplacian(MeshType &m, int step, bool SmoothSelected=false, vcg::CallBackPos * cb=0)
{
  VertexIterator vi;
	LaplacianInfo lpz(CoordType(0,0,0),0);
	SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert,lpz);
	for(int i=0;i<step;++i)
		{
			if(cb)cb(100*i/step, "Classic Laplacian Smoothing");
			TD.Init(lpz);
			AccumulateLaplacianInfo(m,TD);
			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].cnt>0 )
				{
							if(!SmoothSelected || (*vi).IsS())
									(*vi).P() = ( (*vi).P() + TD[*vi].sum)/(TD[*vi].cnt+1);
				}
		}
}

// Same of above but moves only the vertices that do not change FaceOrientation more that the given threshold
static void VertexCoordPlanarLaplacian(MeshType &m, int step, float AngleThrRad = math::ToRad(1.0), bool SmoothSelected=false, vcg::CallBackPos * cb=0)
{
  VertexIterator vi;
	FaceIterator fi;
	LaplacianInfo lpz(CoordType(0,0,0),0);
	SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert,lpz);
	for(int i=0;i<step;++i)
		{
		if(cb)cb(100*i/step, "Planar Laplacian Smoothing");
		TD.Init(lpz);
		AccumulateLaplacianInfo(m,TD);
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			if(!(*vi).IsD() && TD[*vi].cnt>0 )
			{
				if(!SmoothSelected || (*vi).IsS())
					TD[*vi].sum = ( (*vi).P() + TD[*vi].sum)/(TD[*vi].cnt+1);
			}

		for(fi=m.face.begin();fi!=m.face.end();++fi){
				if(!(*fi).IsD()){
					for (int j = 0; j < 3; ++j) {
						if(Angle( NormalizedNormal(TD[(*fi).V0(j)].sum, (*fi).P1(j), (*fi).P2(j) ),
											NormalizedNormal(   (*fi).P0(j)     , (*fi).P1(j), (*fi).P2(j) ) ) > AngleThrRad )
							TD[(*fi).V0(j)].sum = (*fi).P0(j);
					}
				}
			}
			for(fi=m.face.begin();fi!=m.face.end();++fi){
				if(!(*fi).IsD()){
					for (int j = 0; j < 3; ++j) {
						if(Angle( NormalizedNormal(TD[(*fi).V0(j)].sum, TD[(*fi).V1(j)].sum, (*fi).P2(j) ),
											NormalizedNormal(   (*fi).P0(j)     ,    (*fi).P1(j),      (*fi).P2(j) ) ) > AngleThrRad )
						{
							TD[(*fi).V0(j)].sum = (*fi).P0(j);
							TD[(*fi).V1(j)].sum = (*fi).P1(j);
						}
					}
				}
			}

			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].cnt>0 )
						(*vi).P()= TD[*vi].sum;



		}// end step


}

static void VertexCoordLaplacianBlend(MeshType &m, int step, float alpha, bool SmoothSelected=false)
{
	VertexIterator vi;
	LaplacianInfo lpz(CoordType(0,0,0),0);
	assert (alpha<= 1.0);
	SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert);

	for(int i=0;i<step;++i)
		{
		TD.Init(lpz);
		AccumulateLaplacianInfo(m,TD);
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			if(!(*vi).IsD() && TD[*vi].cnt>0 )
			{
				if(!SmoothSelected || (*vi).IsS())
				{
					CoordType Delta = TD[*vi].sum/TD[*vi].cnt - (*vi).P();
					(*vi).P() = (*vi).P() + Delta*alpha;
				}
			}
		}
}

/* a couple of notes about the lambda mu values
We assume that 0 < lambda , and mu  is a negative scale factor such that mu < - lambda.
Holds mu+lambda < 0 (e.g in absolute value mu is greater)

let kpb be the pass-band frequency, taubin says that:
			kpb = 1/lambda + 1/mu >0

Values of kpb from 0.01 to 0.1 produce good results according to the original paper.

kpb * mu - mu/lambda = 1
mu = 1/(kpb-1/lambda )

So if
* lambda == 0.5, kpb==0.1  -> mu = 1/(0.1 - 2)  = -0.526
* lambda == 0.5, kpb==0.01 -> mu = 1/(0.01 - 2) = -0.502
*/


static void VertexCoordTaubin(MeshType &m, int step, float lambda, float mu, bool SmoothSelected=false, vcg::CallBackPos * cb=0)
{
	LaplacianInfo lpz(CoordType(0,0,0),0);
	SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert,lpz);
	VertexIterator vi;
	for(int i=0;i<step;++i)
		{
		  if(cb) cb(100*i/step, "Taubin Smoothing");
			TD.Init(lpz);
			AccumulateLaplacianInfo(m,TD);
			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].cnt>0 )
				{
					if(!SmoothSelected || (*vi).IsS())
					{
						CoordType Delta = TD[*vi].sum/TD[*vi].cnt - (*vi).P();
						(*vi).P() = (*vi).P() + Delta*lambda ;
					}
				}
			TD.Init(lpz);
			AccumulateLaplacianInfo(m,TD);
			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
					if(!(*vi).IsD() && TD[*vi].cnt>0 )
					{
						if(!SmoothSelected || (*vi).IsS())
						{
							CoordType Delta = TD[*vi].sum/TD[*vi].cnt - (*vi).P();
							(*vi).P() = (*vi).P() + Delta*mu ;
						}
					}
			} // end for step
}


static void VertexCoordLaplacianQuality(MeshType &m, int step, bool SmoothSelected=false)
{
  LaplacianInfo lpz;
	lpz.sum=CoordType(0,0,0);
	lpz.cnt=1;
	SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert,lpz);
	for(int i=0;i<step;++i)
		{
			for(VertexIterator vi=m.vert.begin();vi!=m.vert.end();++vi)
					if(!(*vi).IsD() && TD[*vi].cnt>0 )
						if(!SmoothSelected || (*vi).IsS())
						{
							float q=(*vi).Q();
							(*vi).P()=(*vi).P()*q + (TD[*vi].sum/TD[*vi].cnt)*(1.0-q);
						}
		} // end for
};

/*
  Improved Laplacian Smoothing of Noisy Surface Meshes
  J. Vollmer, R. Mencl, and H. M�ller
  EUROGRAPHICS Volume 18 (1999), Number 3
*/

class HCSmoothInfo
{
public:
	CoordType dif;
	CoordType sum;
	int cnt;
};

static void VertexCoordLaplacianHC(MeshType &m, int step, bool SmoothSelected=false )
{
	ScalarType beta=0.5;
  HCSmoothInfo lpz;
	lpz.sum=CoordType(0,0,0);
	lpz.dif=CoordType(0,0,0);
	lpz.cnt=0;
		for(int i=0;i<step;++i)
		{
			SimpleTempData<typename MeshType::VertContainer,HCSmoothInfo > TD(m.vert,lpz);
			// First Loop compute the laplacian
			FaceIterator fi;
			for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
				{
					for(int j=0;j<3;++j)
					{
						TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
						TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
						++TD[(*fi).V(j)].cnt;
						++TD[(*fi).V1(j)].cnt;
						// se l'edge j e' di bordo si deve sommare due volte
						if((*fi).IsB(j))
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
						}
					}
				}
			VertexIterator vi;
			for(vi=m.vert.begin();vi!=m.vert.end();++vi) if(!(*vi).IsD())
				 TD[*vi].sum/=(float)TD[*vi].cnt;

			// Second Loop compute average difference
			for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
				{
					for(int j=0;j<3;++j)
					{
						TD[(*fi).V(j)].dif +=TD[(*fi).V1(j)].sum-(*fi).V1(j)->P();
						TD[(*fi).V1(j)].dif+=TD[(*fi).V(j)].sum-(*fi).V(j)->P();
						// se l'edge j e' di bordo si deve sommare due volte
						if((*fi).IsB(j))
						{
							TD[(*fi).V(j)].dif +=TD[(*fi).V1(j)].sum-(*fi).V1(j)->P();
							TD[(*fi).V1(j)].dif+=TD[(*fi).V(j)].sum-(*fi).V(j)->P();
						}
					}
				}

			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				{
				 TD[*vi].dif/=(float)TD[*vi].cnt;
				 if(!SmoothSelected || (*vi).IsS())
						(*vi).P()= TD[*vi].sum - (TD[*vi].sum - (*vi).P())*beta  + (TD[*vi].dif)*(1.f-beta);
				}
		} // end for step
};

// Laplacian smooth of the quality.


class ColorSmoothInfo
{
public:
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
	int cnt;
};

static void VertexColorLaplacian(MeshType &m, int step, bool SmoothSelected=false, vcg::CallBackPos * cb=0)
{
	ColorSmoothInfo csi;
	csi.r=0; csi.g=0; csi.b=0; csi.cnt=0;
	SimpleTempData<typename MeshType::VertContainer, ColorSmoothInfo> TD(m.vert,csi);

	for(int i=0;i<step;++i)
	{
		if(cb) cb(100*i/step, "Vertex Color Laplacian Smoothing");
		VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			TD[*vi]=csi;

		FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j))
					{
						TD[(*fi).V(j)].r+=(*fi).V1(j)->C()[0];
						TD[(*fi).V(j)].g+=(*fi).V1(j)->C()[1];
						TD[(*fi).V(j)].b+=(*fi).V1(j)->C()[2];
						TD[(*fi).V(j)].a+=(*fi).V1(j)->C()[3];

						TD[(*fi).V1(j)].r+=(*fi).V(j)->C()[0];
						TD[(*fi).V1(j)].g+=(*fi).V(j)->C()[1];
						TD[(*fi).V1(j)].b+=(*fi).V(j)->C()[2];
						TD[(*fi).V1(j)].a+=(*fi).V(j)->C()[3];

						++TD[(*fi).V(j)].cnt;
						++TD[(*fi).V1(j)].cnt;
					}

		// si azzaera i dati per i vertici di bordo
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if((*fi).IsB(j))
					{
						TD[(*fi).V(j)]=csi;
						TD[(*fi).V1(j)]=csi;
					}

		// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if((*fi).IsB(j))
					{
						TD[(*fi).V(j)].r+=(*fi).V1(j)->C()[0];
						TD[(*fi).V(j)].g+=(*fi).V1(j)->C()[1];
						TD[(*fi).V(j)].b+=(*fi).V1(j)->C()[2];
						TD[(*fi).V(j)].a+=(*fi).V1(j)->C()[3];

						TD[(*fi).V1(j)].r+=(*fi).V(j)->C()[0];
						TD[(*fi).V1(j)].g+=(*fi).V(j)->C()[1];
						TD[(*fi).V1(j)].b+=(*fi).V(j)->C()[2];
						TD[(*fi).V1(j)].a+=(*fi).V(j)->C()[3];

						++TD[(*fi).V(j)].cnt;
						++TD[(*fi).V1(j)].cnt;
					}

		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			if(!(*vi).IsD() && TD[*vi].cnt>0 )
				if(!SmoothSelected || (*vi).IsS())
				{
					(*vi).C()[0] = (unsigned int) ceil((double) (TD[*vi].r / TD[*vi].cnt));
					(*vi).C()[1] = (unsigned int) ceil((double) (TD[*vi].g / TD[*vi].cnt));
					(*vi).C()[2] = (unsigned int) ceil((double) (TD[*vi].b / TD[*vi].cnt));
					(*vi).C()[3] = (unsigned int) ceil((double) (TD[*vi].a / TD[*vi].cnt));
				}
	} // end for step
};

static void FaceColorLaplacian(MeshType &m, int step, bool SmoothSelected=false, vcg::CallBackPos * cb=0)
{
	ColorSmoothInfo csi;
	csi.r=0; csi.g=0; csi.b=0; csi.cnt=0;
	SimpleTempData<typename MeshType::FaceContainer, ColorSmoothInfo> TD(m.face,csi);

	for(int i=0;i<step;++i)
	{
		if(cb) cb(100*i/step, "Face Color Laplacian Smoothing");
		FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			TD[*fi]=csi;

		for(fi=m.face.begin();fi!=m.face.end();++fi)
		{
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j))
					{
						TD[*fi].r+=(*fi).FFp(j)->C()[0];
						TD[*fi].g+=(*fi).FFp(j)->C()[1];
						TD[*fi].b+=(*fi).FFp(j)->C()[2];
						TD[*fi].a+=(*fi).FFp(j)->C()[3];
						++TD[*fi].cnt;
					}
		}
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD() && TD[*fi].cnt>0 )
				if(!SmoothSelected || (*fi).IsS())
				{
					(*fi).C()[0] = (unsigned int) ceil((float) (TD[*fi].r / TD[*fi].cnt));
					(*fi).C()[1] = (unsigned int) ceil((float) (TD[*fi].g / TD[*fi].cnt));
					(*fi).C()[2] = (unsigned int) ceil((float) (TD[*fi].b / TD[*fi].cnt));
					(*fi).C()[3] = (unsigned int) ceil((float) (TD[*fi].a / TD[*fi].cnt));
				}
	} // end for step
};

// Laplacian smooth of the quality.

class QualitySmoothInfo
{
public:
	ScalarType sum;
	int cnt;
};


static void VertexQualityLaplacian(MeshType &m, int step=1, bool SmoothSelected=false)
{
  QualitySmoothInfo lpz;
	lpz.sum=0;
	lpz.cnt=0;
	SimpleTempData<typename MeshType::VertContainer,QualitySmoothInfo> TD(m.vert,lpz);
	//TD.Start(lpz);
	for(int i=0;i<step;++i)
	{
		VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;

		FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j))
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->Q();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->Q();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)]=lpz;
								TD[(*fi).V1(j)]=lpz;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->Q();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->Q();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}

	//VertexIterator vi;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			if(!SmoothSelected || (*vi).IsS())
					(*vi).Q()=TD[*vi].sum/TD[*vi].cnt;
	}

	//TD.Stop();
};

static void VertexNormalLaplacian(MeshType &m, int step,bool SmoothSelected=false)
{
    LaplacianInfo lpz;
      lpz.sum=CoordType(0,0,0);
      lpz.cnt=0;
    SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert,lpz);
    for(int i=0;i<step;++i)
	{
		VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;

		FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j))
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->N();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->N();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)]=lpz;
								TD[(*fi).V1(j)]=lpz;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->N();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->N();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}

	//VertexIterator vi;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			if(!SmoothSelected || (*vi).IsS())
					(*vi).N()=TD[*vi].sum/TD[*vi].cnt;
	}

};

// Smooth solo lungo la direzione di vista
	// alpha e' compreso fra 0(no smoot) e 1 (tutto smoot)
  // Nota che se smootare il bordo puo far fare bandierine.
static void VertexCoordViewDepth(MeshType &m,
				 const CoordType & viewpoint,
				 const ScalarType alpha,
				 int step, bool SmoothBorder=false )
{
	LaplacianInfo lpz;
	lpz.sum=CoordType(0,0,0);
	lpz.cnt=0;
	SimpleTempData<typename MeshType::VertContainer,LaplacianInfo > TD(m.vert,lpz);
	for(int i=0;i<step;++i)
	{
		VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;

		FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j))
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->cP();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->cP();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)]=lpz;
								TD[(*fi).V1(j)]=lpz;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
     if(SmoothBorder)
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->cP();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->cP();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			{
				CoordType np = TD[*vi].sum/TD[*vi].cnt;
				CoordType d = (*vi).cP() - viewpoint; d.Normalize();
				ScalarType s = d .dot ( np - (*vi).cP() );
				(*vi).P() += d * (s*alpha);
			}
	}
}



/****************************************************************************************************************/
/****************************************************************************************************************/
// Paso Double Smoothing
//  The proposed
// approach is a two step method where  in the first step the face normals
// are adjusted and then, in a second phase, the vertex positions are updated.
/****************************************************************************************************************/
/****************************************************************************************************************/
// Classi di info

class PDVertInfo
{
public:
	CoordType np;
};


class PDFaceInfo
{
public:
	CoordType m;
};
/***************************************************************************/
// Paso Doble Step 1 compute the smoothed normals
/***************************************************************************/
// Requirements:
// VF Topology
// Normalized Face Normals
//
// This is the Normal Smoothing approach of Shen and Berner
// Fuzzy Vector Median-Based Surface Smoothing TVCG 2004


void FaceNormalFuzzyVectorSB(MeshType &m,
				  SimpleTempData<typename MeshType::FaceContainer,PDFaceInfo > &TD,
				  ScalarType sigma)
{
	int i;

	FaceIterator fi;

	for(fi=m.face.begin();fi!=m.face.end();++fi)
	{
		CoordType bc=(*fi).Barycenter();
    // 1) Clear all the visited flag of faces that are vertex-adjacent to fi
		for(i=0;i<3;++i)
		{
    	vcg::face::VFIterator<FaceType> ep(&*fi,i);
		  while (!ep.End())
			{
				ep.f->ClearV();
				++ep;
			}
		}

    // 1) Effectively average the normals weighting them with
    (*fi).SetV();
		CoordType mm=CoordType(0,0,0);
		for(i=0;i<3;++i)
		{
		  vcg::face::VFIterator<FaceType> ep(&*fi,i);
		  while (!ep.End())
			{
				if(! (*ep.f).IsV() )
				{
					if(sigma>0)
					{
						ScalarType dd=SquaredDistance(ep.f->Barycenter(),bc);
						ScalarType ang=AngleN(ep.f->N(),(*fi).N());
						mm+=ep.f->N()*exp((-sigma)*ang*ang/dd);
					}
					else mm+=ep.f->N();
					(*ep.f).SetV();
				}
				++ep;
			}
		}
		mm.Normalize();
		TD[*fi].m=mm;
	}
}

// Replace the normal of the face with the average of normals of the vertex adijacent faces.
// Normals are weighted with face area.
// It assumes that:
// Normals are normalized:
// VF adjacency is present.

static void FaceNormalLaplacianVF(MeshType &m)
{
	SimpleTempData<typename MeshType::FaceContainer, PDFaceInfo> TDF(m.face);

	PDFaceInfo lpzf;
	lpzf.m=CoordType(0,0,0);

	assert(tri::HasVFAdjacency(m));
	TDF.Start(lpzf);
	int i;

  FaceIterator fi;

	tri::UpdateNormals<MeshType>::AreaNormalizeFace(m);

	for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
	{
				CoordType bc=Barycenter<FaceType>(*fi);
				// 1) Clear all the visited flag of faces that are vertex-adjacent to fi
				for(i=0;i<3;++i)
				{
					VFLocalIterator ep(&*fi,i);
					for (;!ep.End();++ep)
						ep.f->ClearV();
				}

				// 2) Effectively average the normals
				CoordType normalSum=(*fi).N();

				for(i=0;i<3;++i)
				{
					VFLocalIterator ep(&*fi,i);
					for (;!ep.End();++ep)
					{
						if(! (*ep.f).IsV() )
								{
										 normalSum += ep.f->N();
										 (*ep.f).SetV();
								}
					}
				}
				normalSum.Normalize();
				TDF[*fi].m=normalSum;
	}
	for(fi=m.face.begin();fi!=m.face.end();++fi)
		(*fi).N()=TDF[*fi].m;

	tri::UpdateNormals<MeshType>::NormalizeFace(m);

	TDF.Stop();
}

// Replace the normal of the face with the average of normals of the face adijacent faces.
// Normals are weighted with face area.
// It assumes that:
// Normals are normalized:
// FF adjacency is present.


static void FaceNormalLaplacianFF(MeshType &m, int step=1, bool SmoothSelected=false )
{
	PDFaceInfo lpzf;
	lpzf.m=CoordType(0,0,0);
	SimpleTempData<typename MeshType::FaceContainer, PDFaceInfo> TDF(m.face,lpzf);
	assert(tri::HasFFAdjacency(m));

  FaceIterator fi;
	tri::UpdateNormals<MeshType>::AreaNormalizeFace(m);
  for(int i=0;i<step;++i)
		{
				for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
				{
							CoordType normalSum=(*fi).N();

							for(i=0;i<3;++i)
										normalSum+=(*fi).FFp(i)->N();

							TDF[*fi].m=normalSum;
				}
				for(fi=m.face.begin();fi!=m.face.end();++fi)
						if(!SmoothSelected || (*fi).IsS())
								(*fi).N()=TDF[*fi].m;

				tri::UpdateNormals<MeshType>::NormalizeFace(m);
		}
}


/***************************************************************************/
// Paso Doble Step 1 compute the smoothed normals
/***************************************************************************/
// Requirements:
// VF Topology
// Normalized Face Normals
//
// This is the Normal Smoothing approach bsased on a angle thresholded weighting
// sigma is in the 0 .. 1 range, it represent the cosine of a threshold angle.
// sigma == 0 All the normals are averaged
// sigma == 1 Nothing is averaged.
// Only within the specified range are averaged toghether. The averagin is weighted with the


static void FaceNormalAngleThreshold(MeshType &m,
				  SimpleTempData<typename MeshType::FaceContainer,PDFaceInfo> &TD,
				  ScalarType sigma)
{
	int i;


  FaceIterator fi;

	for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
	{
		CoordType bc=Barycenter<FaceType>(*fi);
    // 1) Clear all the visited flag of faces that are vertex-adjacent to fi
		for(i=0;i<3;++i)
		{
    	VFLocalIterator ep(&*fi,i);
		  for (;!ep.End();++ep)
				ep.f->ClearV();
		}

    // 1) Effectively average the normals weighting them with the squared difference of the angle similarity
		// sigma is the cosine of a threshold angle. sigma \in 0..1
		// sigma == 0 All the normals are averaged
		// sigma == 1 Nothing is averaged.
		// The averaging is weighted with the difference between normals. more similar the normal more important they are.

    CoordType normalSum=CoordType(0,0,0);
		for(i=0;i<3;++i)
		{
    	VFLocalIterator ep(&*fi,i);
		  for (;!ep.End();++ep)
			{
				if(! (*ep.f).IsV() )
				{
          ScalarType cosang=ep.f->N().dot((*fi).N());
					// Note that if two faces form an angle larger than 90 deg, their contribution should be very very small.
					// Without this clamping
					cosang = math::Clamp(cosang,0.0001f,1.f);
          if(cosang >= sigma)
          {
            ScalarType w = cosang-sigma;
						normalSum += ep.f->N()*(w*w);   // similar normals have a cosang very close to 1 so cosang - sigma is maximized
          }
					(*ep.f).SetV();
				}
			}
		}
		normalSum.Normalize();
		TD[*fi].m=normalSum;
	}

  for(fi=m.face.begin();fi!=m.face.end();++fi)
    (*fi).N()=TD[*fi].m;
}

/****************************************************************************************************************/
// Restituisce il gradiente dell'area del triangolo nel punto p.
// Nota che dovrebbe essere sempre un vettore che giace nel piano del triangolo e perpendicolare al lato opposto al vertice p.
// Ottimizzato con Maple e poi pesantemente a mano.

CoordType TriAreaGradient(CoordType &p,CoordType &p0,CoordType &p1)
{
	CoordType dd = p1-p0;
	CoordType d0 = p-p0;
	CoordType d1 = p-p1;
	CoordType grad;

	ScalarType t16 =  d0[1]* d1[2] - d0[2]* d1[1];
	ScalarType t5  = -d0[2]* d1[0] + d0[0]* d1[2];
	ScalarType t4  = -d0[0]* d1[1] + d0[1]* d1[0];

	ScalarType delta= sqrtf(t4*t4 + t5*t5 +t16*t16);

	grad[0]= (t5  * (-dd[2]) + t4 * ( dd[1]))/delta;
	grad[1]= (t16 * (-dd[2]) + t4 * (-dd[0]))/delta;
	grad[2]= (t16 * ( dd[1]) + t5 * ( dd[0]))/delta;

	return grad;
}

template <class ScalarType>
CoordType CrossProdGradient(CoordType &p, CoordType &p0, CoordType &p1, CoordType &m)
{
	CoordType grad;
	CoordType p00=p0-p;
	CoordType p01=p1-p;
	grad[0] = (-p00[2] + p01[2])*m[1] + (-p01[1] + p00[1])*m[2];
	grad[1] = (-p01[2] + p00[2])*m[0] + (-p00[0] + p01[0])*m[2];
	grad[2] = (-p00[1] + p01[1])*m[0] + (-p01[0] + p00[0])*m[1];

	return grad;
}

/*
Deve Calcolare il gradiente di
E(p) = A(p,p0,p1) (n - m)^2 =
A(...) (2-2nm)   =
(p0-p)^(p1-p)
2A - 2A * ------------- m  =
2A

2A  -  2 (p0-p)^(p1-p) * m
*/

CoordType FaceErrorGrad(CoordType &p,CoordType &p0,CoordType &p1, CoordType &m)
{
	return     TriAreaGradient(p,p0,p1) *2.0f
		- CrossProdGradient(p,p0,p1,m) *2.0f ;
}
/***************************************************************************/
// Paso Doble Step 2 Fitta la mesh a un dato insieme di normali
/***************************************************************************/


void FitMesh(MeshType &m,
			 SimpleTempData<typename MeshType::VertContainer, PDVertInfo> &TDV,
			 SimpleTempData<typename MeshType::FaceContainer, PDFaceInfo> &TDF,
			 float lambda)
{
	//vcg::face::Pos<FaceType> ep;
	vcg::face::VFIterator<FaceType> ep;
	VertexIterator vi;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
	{
		CoordType ErrGrad=CoordType(0,0,0);

		ep.f=(*vi).VFp();
		ep.z=(*vi).VFi();
		while (!ep.End())
		{
			ErrGrad+=FaceErrorGrad(ep.f->V(ep.z)->P(),ep.f->V1(ep.z)->P(),ep.f->V2(ep.z)->P(),TDF[ep.f].m);
			++ep;
		}
		TDV[*vi].np=(*vi).P()-ErrGrad*(ScalarType)lambda;
	}

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		(*vi).P()=TDV[*vi].np;

}
/****************************************************************************************************************/



static void FastFitMesh(MeshType &m,
			 SimpleTempData<typename MeshType::VertContainer, PDVertInfo> &TDV,
			 SimpleTempData<typename MeshType::FaceContainer, PDFaceInfo> &TDF,
			 bool OnlySelected=false)
{
	//vcg::face::Pos<FaceType> ep;
	vcg::face::VFIterator<FaceType> ep;
	VertexIterator vi;

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
	{
   CoordType Sum(0,0,0);
   ScalarType cnt=0;
   VFLocalIterator ep(&*vi);
	 for (;!ep.End();++ep)
		{
      CoordType bc=Barycenter<FaceType>(*ep.F());
      Sum += ep.F()->N()*(ep.F()->N().dot(bc - (*vi).P()));
      ++cnt;
		}
		TDV[*vi].np=(*vi).P()+ Sum*(1.0/cnt);
	}

	if(OnlySelected)
	{
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if((*vi).IsS()) (*vi).P()=TDV[*vi].np;
	}
	else
	{
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		(*vi).P()=TDV[*vi].np;
	}
}



static void VertexCoordPasoDoble(MeshType &m, int step, typename MeshType::ScalarType Sigma=0, int FitStep=10, typename MeshType::ScalarType FitLambda=0.05)
{
	SimpleTempData< typename MeshType::VertContainer, PDVertInfo> TDV(m.vert);
	SimpleTempData< typename MeshType::FaceContainer, PDFaceInfo> TDF(m.face);
	PDVertInfo lpzv;
	lpzv.np=CoordType(0,0,0);
	PDFaceInfo lpzf;
	lpzf.m=CoordType(0,0,0);

	assert(m.HasVFTopology());
	m.HasVFTopology();
	TDV.Start(lpzv);
	TDF.Start(lpzf);
	for(int j=0;j<step;++j)
	{

		vcg::tri::UpdateNormals<MeshType>::PerFace(m);
		FaceNormalAngleThreshold(m,TDF,Sigma);
		for(int k=0;k<FitStep;k++)
			FitMesh(m,TDV,TDF,FitLambda);
	}

	TDF.Stop();
	TDV.Stop();

}

// The sigma parameter affect the normal smoothing step

static void VertexCoordPasoDobleFast(MeshType &m, int NormalSmoothStep, typename MeshType::ScalarType Sigma=0, int FitStep=50, bool SmoothSelected =false)
{
	PDVertInfo lpzv;
	lpzv.np=CoordType(0,0,0);
	PDFaceInfo lpzf;
	lpzf.m=CoordType(0,0,0);

	assert(HasVFAdjacency(m));
	SimpleTempData< typename MeshType::VertContainer, PDVertInfo> TDV(m.vert,lpzv);
	SimpleTempData< typename MeshType::FaceContainer, PDFaceInfo> TDF(m.face,lpzf);

  for(int j=0;j<NormalSmoothStep;++j)
	   FaceNormalAngleThreshold(m,TDF,Sigma);

  for(int j=0;j<FitStep;++j)
	  FastFitMesh(m,TDV,TDF,SmoothSelected);
}

}; //end Smooth class

}		// End namespace tri
}		// End namespace vcg

#endif //  VCG_SMOOTH
