//#
//# FILE: lemke.cc -- Lemke-Howson module
//#
//# $Id$
//#

#include "rational.h"
#include "gwatch.h"
#include "gpvector.h"

#include "nfg.h"

#include "lemke.h"

template <class T> gMatrix<T> Make_A1(const Nfg<T> &, const NFSupport &);
template <class T> gVector<T> Make_b1(const Nfg<T> &, const NFSupport &);
template <class T> gMatrix<T> Make_A2(const Nfg<T> &, const NFSupport &);
template <class T> gVector<T> Make_b2(const Nfg<T> &, const NFSupport &);


//---------------------------------------------------------------------------
//                        LemkeParams: member functions
//---------------------------------------------------------------------------

LemkeParams::LemkeParams(gStatus &status_) 
  : dup_strat(0), trace(0), stopAfter(0), tracefile(&gnull), status(status_)
{ }

//---------------------------------------------------------------------------
//                        LemkeModule: member functions
//---------------------------------------------------------------------------

//
// Lemke is the most important routine.
// It implements the Lemke-Howson algorithm, as refined by Eaves.
// It is assumed that the starting point is a complementary basic
// feasible solution.  If not it returns 0 without doing anything.
//


template <class T> int LemkeModule<T>::Lemke(int dup)
{
  BFS<T> cbfs((T) 0);
  int i;

  if (NF.NumPlayers() != 2 || !params.tracefile)   return 0;

  gWatch watch;

  List = BFS_List();

  gMatrix<T> A1(Make_A1(NF, support));
  gVector<T> b1(Make_b1(NF, support));
  gMatrix<T> A2(Make_A2(NF, support));
  gVector<T> b2(Make_b2(NF, support));
  LHTableau<T> B(A1,A2,b1,b2);
   if (dup == 0)
    All_Lemke(0,B,npivots);
  else  {
    B.LemkePath(dup);
    Add_BFS(B);
  }
  if (params.trace >= 2)  {
    for (i = 1; i <= List.Length(); i++)   {
      List[i].Dump(*params.tracefile);
      (*params.tracefile) << "\n";
    }
    
  }
//  if(params.trace >= 1)
//    (*params.tracefile) << "\nN Pivots = " << npivots << "\n";
  
  AddSolutions();
  time = watch.Elapsed();
  return List.Length();
}

template <class T> int LemkeModule<T>::Add_BFS(LHTableau<T> &B)
{
  BFS<T> cbfs((T) 0);
/*
  gVector<T> v(B.MinRow(), B.MaxRow());
  B.BasisVector(v);

  for (int i = B.MinCol(); i <= B.MaxCol(); i++)
    if (B.Member(i)) {
      cbfs.Define(i, v[B.Find(i)]);
    }
*/
  cbfs = B.GetBFS();
  if (List.Contains(cbfs))  return 0;
  if(params.trace >=2) {
    (*params.tracefile) << "\nFound CBFS";
    (*params.tracefile)  << "\nB = ";
    B.Dump(*params.tracefile);
    (*params.tracefile)  << "\ncbfs = ";
    cbfs.Dump(*params.tracefile );
  }
  List.Append(cbfs);
  return 1;
}

//
// All_Lemke finds all accessible Nash equilibria by recursively 
// calling itself.  List maintains the list of basic variables 
// for the equilibria that have already been found.  
// From each new accessible equilibrium, it follows
// all possible paths, adding any new equilibria to the List.  
//
template <class T> int LemkeModule<T>::All_Lemke(int j, LHTableau<T> &B,long &np)
{
//  BFS<T> cbfs((T) 0);
  int i;

  np+=B.NumPivots();
  if(!Add_BFS(B)) return 1;
  
  for (i = B.MinCol(); i <= B.MaxCol(); i++)
    if (i != j)  {
//      gout << "\n i = " << i;
      LHTableau<T> Bcopy(B);
//      Bcopy.NumPivots();
      Bcopy.LemkePath(i);
      All_Lemke(i,Bcopy,np);
//      gout << "\nend of AllLemke: " << i;
//      delete Bcopy;

    }
  return 1;
}

template <class T>
const gList<MixedSolution<T> > &LemkeModule<T>::GetSolutions(void) const
{
  return solutions;
}

template <class T> void LemkeModule<T>::AddSolutions(void)
{
  int i,j;
  int n1=NF.NumStrats(1);
  int n2=NF.NumStrats(2);
  solutions.Flush();

  for (i = 1; i <= List.Length(); i++)    {
    MixedProfile<T> profile(NF);
    T sum = (T) 0;

    for (j = 1; j <= n1; j++)
      if (List[i].IsDefined(j))   sum += List[i](j);

    if (sum == (T) 0)  continue;

    for (j = 1; j <= n1; j++) 
      if (List[i].IsDefined(j))   profile(1, j) = List[i](j) / sum;
      else  profile(1, j) = (T) 0;

    sum = (T) 0;

    for (j = 1; j <= n2; j++)
      if (List[i].IsDefined(n1 + j))  
	sum += List[i](n1 + j);

    if (sum == (T) 0)  continue;

    for (j = 1; j <= n2; j++)
      if (List[i].IsDefined(n1 + j))
	profile(2, j) = List[i](n1 + j) / sum;
      else
	profile(2, j) = (T) 0;

    int index;
    bool add = false;
    if((params.status.Get() !=1) ||
       (params.status.Get() ==1 && profile.IsNash()))
      add = true;
    if(add) {
      index = solutions.Append(MixedSolution<T>(profile, id_LEMKE));
      if(params.status.Get() != 1) {
	solutions[index].SetIsNash(T_YES);
	solutions[index].SetIsPerfect(T_YES);
      }
    }
  }
}

template <class T> long LemkeModule<T>::NumPivots(void) const
{
  return npivots;
}

//-------------------------------------------------------------------------
//                    LemkeModule<T>: Member functions
//-------------------------------------------------------------------------

template <class T>
LemkeModule<T>::LemkeModule(const Nfg<T> &N, const LemkeParams &p,
			    const NFSupport &S)
  : NF(N), support(S), params(p), npivots(0)
{ }

template <class T> LemkeModule<T>::~LemkeModule()
{ }

template <class T> double LemkeModule<T>::Time(void) const
{
  return time;
}

#ifdef __GNUG__
template class LemkeModule<double>;
template class LemkeModule<gRational>;
#elif defined __BORLANDC__
#pragma option -Jgd
class LemkeModule<double>;
class LemkeModule<gRational>;
#pragma option -Jgx
#endif   // __GNUG__, __BORLANDC__


//-------------------------------------------------------------------------
//                    Convenience functions for Lemke
//-------------------------------------------------------------------------

template <class T>
int Lemke(const Nfg<T> &N, const LemkeParams &p,
	  gList<MixedSolution<T> > &solutions,
	  long &npivots, gRational &time)
{
  NFSupport S(N);
  LemkeModule<T> LM(N, p, S);
  int result = LM.Lemke();

  npivots = LM.NumPivots();
  time = LM.Time();
  
//   LM.GetSolutions(solutions);
  solutions = LM.GetSolutions();

  return result;
}

#ifdef __GNUG__
template int Lemke(const Nfg<double> &, const LemkeParams &,
		   gList<MixedSolution<double> > &, long &, gRational &);
template int Lemke(const Nfg<gRational> &, const LemkeParams &,
		   gList<MixedSolution<gRational> > &, long &, gRational &);
#elif defined __BORLANDC__
#pragma option -Jgd
int Lemke(const Nfg<double> &, const LemkeParams &,
	  gList<MixedSolution<double> > &, long &, gRational &);
int Lemke(const Nfg<gRational> &, const LemkeParams &,
	  gList<MixedSolution<gRational> > &, long &, gRational &);
#pragma option -Jgx
#endif   // __GNUG__, __BORLANDC__
