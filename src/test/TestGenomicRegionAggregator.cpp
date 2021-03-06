/**
  \file TestGenomicRegionAggregator
  \brief TODO

  \authors Philip J. Uren, Andrew D. Smith

  \section copyright Copyright Details
  Copyright (C) 2012
  University of Southern California,
  Philip J. Uren, Andrew D. Smith

  \section license License Details
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  \section bugs Known Bugs

  \section history Revision History
**/

#include "gtest/gtest.h"
#include "GenomicRegionAggregator.hpp"

#include <vector>
#include <sstream>

using std::endl;
using std::vector;
using std::stringstream;



/**
 * \brief TODO
 */
TEST(GRegAggTest, test_agg_clusters_pvals_covars) {
  vector<GenomicRegion> regions;
  vector<GenomicRegion> expect_d0_regions, expect_d1_regions, expect_d2_regions;
  regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 8,  9,  "C1R7",  6, '+'));
  regions.push_back(GenomicRegion("chr1", 9,  10, "C1R8",  8, '+'));
  regions.push_back(GenomicRegion("chr1", 10, 11, "C1R9",  1, '+'));
  // ****
  regions.push_back(GenomicRegion("chr2", 3, 4,  "C2R0",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  regions.push_back(GenomicRegion("chr2", 5, 6,  "C2R2",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 6, 7,  "C2R3",  7, '+'));
  regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  regions.push_back(GenomicRegion("chr2", 8, 9,  "C2R5",  5, '+'));
  regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // p-values
  const double pvs[17] = {0.02,  0.0005, 0.001, 0.001, 0.05,
                          0.002, 0.001,  0.05,  0.05,  0.05,
                          0.002, 0.001,  0.002, 0.2,   0.001,
                          0.2,   0.005};
  const vector<double> pvals(pvs, pvs+17);

  // covariates
  const double c1[17] = {1,2,5,2,7, 1,2,3,4,10, 1,1,2,5,7, 1,1};
  const double c2[17] = {2,2,7,1,5, 2,2,1,9,11, 2,1,2,7,8, 7,3};
  vector< vector<double> > cvs;
  cvs.push_back(vector<double>(c1,c1+17));
  cvs.push_back(vector<double>(c2,c2+17));

  // expected regions with cluster distance of 0
  expect_d0_regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 3,  4,  "C2R0",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 4,  5,  "C2R1",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 5,  6,  "C2R2",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 7,  8,  "C2R4",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 9,  10, "C2R6",  2, '+'));

  // expected regions with cluster distance of 1
  expect_d1_regions.push_back(GenomicRegion("chr1", 2, 5,  "X",     3, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr1", 6, 8,  "X",     3, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 3, 6,  "X",     2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // expected regions with cluster distance of 2
  expect_d2_regions.push_back(GenomicRegion("chr1", 2, 8,  "X",
                                            (2+5+2+4+2+4) / (double) 6, '+'));
  expect_d2_regions.push_back(GenomicRegion("chr2", 3, 10, "X",
                                            (1+4+1+7+2+5+2) / (double) 7, '+'));

  // expected average p-values
  const double d0_apvals[10] = {0.0005, 0.001, 0.001, 0.002, 0.001,
                                0.002,  0.001, 0.002, 0.001, 0.005};
  const double d1_apvals[5] = {(0.0005 + 0.001 + 0.001)/(double)3,
                               (0.002  + 0.001)/(double)2,
                               (0.002  + 0.001 + 0.002)/(double)3,
                               0.001,
                               0.005};
  const double d2_apvals[2] = {(0.0005 + 0.001 + 0.001 + 0.05 + 0.002 +
                                0.001) / double(6),
                               (0.002 + 0.001 + 0.002 + 0.2 + 0.001 +
                                0.2 + 0.005) / double(7)};
  vector<double> expect_d0_pvals(d0_apvals, d0_apvals + 10),
                 expect_d1_pvals(d1_apvals, d1_apvals + 5),
                 expect_d2_pvals(d2_apvals, d2_apvals + 2);

  // expected average covariate values
  const double d0_acvars_c1[10] = {  2,5,2,   1,2,        1,1,2,  7,  1};
  const double d1_acvars_c1[5] = {(2+5+2)/(double)3, (1+2)/(double)2,
                                  (1+1+2)/(double)3, 7, 1};
  const double d2_acvars_c1[2] = {(2+5+2+7+1+2)/(double)6,
                                  (1+1+2+5+7+1+1)/(double)7};
  const double d0_acvars_c2[10] = {  2,7,1,   2,2,        2,1,2,  8,  3};
  const double d1_acvars_c2[5] = {(2+7+1)/(double)3, (2+2)/(double)2,
                                  (2+1+2)/(double)3, 8, 3};
  const double d2_acvars_c2[2] = {(2+7+1+5+2+2)/(double)6,
                                  (2+1+2+7+8+7+3)/(double)7};
  vector<double> expect_d0_acvars_c1(d0_acvars_c1, d0_acvars_c1+10);
  vector<double> expect_d1_acvars_c1(d1_acvars_c1, d1_acvars_c1+5);
  vector<double> expect_d2_acvars_c1(d2_acvars_c1, d2_acvars_c1+2);
  vector<double> expect_d0_acvars_c2(d0_acvars_c2, d0_acvars_c2+10);
  vector<double> expect_d1_acvars_c2(d1_acvars_c2, d1_acvars_c2+5);
  vector<double> expect_d2_acvars_c2(d2_acvars_c2, d2_acvars_c2+2);

  stringstream ss_exp_d0, ss_exp_d1, ss_exp_d2;
  for (size_t i=0; i<expect_d0_regions.size(); i++)
    ss_exp_d0 << expect_d0_regions[i] << "\t" << expect_d0_pvals[i] << "\t"
              << expect_d0_acvars_c1[i] << "\t" << expect_d0_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d1_regions.size(); i++)
    ss_exp_d1 << expect_d1_regions[i] << "\t" << expect_d1_pvals[i] << "\t"
              << expect_d1_acvars_c1[i] << "\t" << expect_d1_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d2_regions.size(); i++)
    ss_exp_d2 << expect_d2_regions[i] << "\t" << expect_d2_pvals[i] << "\t"
              << expect_d2_acvars_c1[i] << "\t" << expect_d2_acvars_c2[i] << endl;

  vector<vector<double>::const_iterator> begins, ends;
  begins.push_back(cvs[0].begin());
  begins.push_back(cvs[1].begin());
  ends.push_back(cvs[0].end());
  ends.push_back(cvs[1].end());


  stringstream ss_d0, ss_d1, ss_d2;
  double alpha = 0.01;
  GenomicRegionAggregator(0).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends,
      alpha, ClusterLimitsPrinter(ss_d0));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends,
      alpha, ClusterLimitsPrinter(ss_d1));
  GenomicRegionAggregator(2).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends,
      alpha, ClusterLimitsPrinter(ss_d2));

  EXPECT_EQ(ss_exp_d0.str(), ss_d0.str());
  EXPECT_EQ(ss_exp_d1.str(), ss_d1.str());
  EXPECT_EQ(ss_exp_d2.str(), ss_d2.str());
}


/**
 * \brief TODO
 */
TEST(GRegAggTest, test_agg_clusters_pvals) {
  vector<GenomicRegion> regions;
  vector<GenomicRegion> expect_d0_regions, expect_d1_regions, expect_d2_regions;
  vector<GenomicRegion> nexpect_d0_regions, nexpect_d1_regions, nexpect_d2_regions;
  regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 8,  9,  "C1R7",  6, '+'));
  regions.push_back(GenomicRegion("chr1", 9,  10, "C1R8",  8, '+'));
  regions.push_back(GenomicRegion("chr1", 10, 11, "C1R9",  1, '+'));
  // ****
  regions.push_back(GenomicRegion("chr2", 3, 4,  "C2R0",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  regions.push_back(GenomicRegion("chr2", 5, 6,  "C2R2",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 6, 7,  "C2R3",  7, '+'));
  regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  regions.push_back(GenomicRegion("chr2", 8, 9,  "C2R5",  5, '+'));
  regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // p-values
  const double pvs[17] = {0.02,  0.0005, 0.001, 0.001, 0.05,
                          0.002, 0.001,  0.05,  0.05,  0.05,
                          0.002, 0.001,  0.002, 0.2,   0.001,
                          0.2,   0.005};
  const vector<double> pvals(pvs, pvs+17);

  // expected regions with cluster distance of 0
  expect_d0_regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 3,  4,  "C2R0",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 4,  5,  "C2R1",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 5,  6,  "C2R2",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 7,  8,  "C2R4",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 9,  10, "C2R6",  2, '+'));
  // expected negative regions with cluster distance of 0
  nexpect_d0_regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  nexpect_d0_regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  nexpect_d0_regions.push_back(GenomicRegion("chr1", 8,  9,  "C1R7",  6, '+'));
  nexpect_d0_regions.push_back(GenomicRegion("chr1", 9,  10, "C1R8",  8, '+'));
  nexpect_d0_regions.push_back(GenomicRegion("chr1", 10, 11, "C1R9",  1, '+'));
  nexpect_d0_regions.push_back(GenomicRegion("chr2", 6,  7,  "C2R3",  7, '+'));
  nexpect_d0_regions.push_back(GenomicRegion("chr2", 8,  9,  "C2R5",  5, '+'));

  // expected regions with cluster distance of 1
  expect_d1_regions.push_back(GenomicRegion("chr1", 2, 5,  "X",     3, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr1", 6, 8,  "X",     3, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 3, 6,  "X",     2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));
  // expected negative regions with cluster distance of 1
  nexpect_d1_regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  nexpect_d1_regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  nexpect_d1_regions.push_back(GenomicRegion("chr1", 8,  11, "X",     5, '+'));
  nexpect_d1_regions.push_back(GenomicRegion("chr2", 6,  7,  "C2R3",  7, '+'));
  nexpect_d1_regions.push_back(GenomicRegion("chr2", 8,  9,  "C2R5",  5, '+'));

  // expected regions with cluster distance of 2
  expect_d2_regions.push_back(GenomicRegion("chr1", 2, 8,  "X",
                                            (2+5+2+4+2+4) / (double) 6, '+'));
  expect_d2_regions.push_back(GenomicRegion("chr2", 3, 10, "X",
                                            (1+4+1+7+2+5+2) / (double) 7, '+'));
  // expected negative regions with cluster distance of 2
  nexpect_d2_regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  nexpect_d2_regions.push_back(GenomicRegion("chr1", 8,  11, "X",     5, '+'));

  // expected average p-values
  const double d0_apvals[10] = {0.0005, 0.001, 0.001, 0.002, 0.001,
                                0.002,  0.001, 0.002, 0.001, 0.005};
  const double d1_apvals[5] = {(0.0005 + 0.001 + 0.001)/(double)3,
                               (0.002  + 0.001)/(double)2,
                               (0.002  + 0.001 + 0.002)/(double)3,
                               0.001,
                               0.005};
  const double d2_apvals[2] = {(0.0005 + 0.001 + 0.001 + 0.05 + 0.002 +
                                0.001) / double(6),
                               (0.002 + 0.001 + 0.002 + 0.2 + 0.001 +
                                0.2 + 0.005) / double(7)};
  vector<double> expect_d0_pvals(d0_apvals, d0_apvals + 10),
                 expect_d1_pvals(d1_apvals, d1_apvals + 5),
                 expect_d2_pvals(d2_apvals, d2_apvals + 2);
  // expected average p-values in the negative clusters
  const double nd0_apvals[7] = {0.02, 0.05, 0.05, 0.05, 0.05, 0.2, 0.2};
  const double nd1_apvals[5] = {0.02, 0.05, (0.05 + 0.05 + 0.05)/(double)3,
                                0.2, 0.2};
  const double nd2_apvals[2] = {0.02, (0.05 + 0.05 + 0.05)/(double)3};
  vector<double> nexpect_d0_pvals(nd0_apvals, nd0_apvals + 7),
                 nexpect_d1_pvals(nd1_apvals, nd1_apvals + 5),
                 nexpect_d2_pvals(nd2_apvals, nd2_apvals + 2);


  stringstream ss_exp_d0, ss_exp_d1, ss_exp_d2;
  for (size_t i=0; i<expect_d0_regions.size(); i++)
    ss_exp_d0 << expect_d0_regions[i] << "\t" << expect_d0_pvals[i] << endl;
  for (size_t i=0; i<expect_d1_regions.size(); i++)
    ss_exp_d1 << expect_d1_regions[i] << "\t" << expect_d1_pvals[i] << endl;
  for (size_t i=0; i<expect_d2_regions.size(); i++)
    ss_exp_d2 << expect_d2_regions[i] << "\t" << expect_d2_pvals[i] << endl;

  stringstream nss_exp_d0, nss_exp_d1, nss_exp_d2;
  for (size_t i=0; i<nexpect_d0_regions.size(); i++)
    nss_exp_d0 << nexpect_d0_regions[i] << "\t" << nexpect_d0_pvals[i] << endl;
  for (size_t i=0; i<nexpect_d1_regions.size(); i++)
    nss_exp_d1 << nexpect_d1_regions[i] << "\t" << nexpect_d1_pvals[i] << endl;
  for (size_t i=0; i<nexpect_d2_regions.size(); i++)
    nss_exp_d2 << nexpect_d2_regions[i] << "\t" << nexpect_d2_pvals[i] << endl;

  stringstream ss_d0, ss_d1, ss_d2;
  stringstream nss_d0, nss_d1, nss_d2;
  double alpha = 0.01;
  GenomicRegionAggregator(0).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(),
      alpha, ClusterLimitsPrinter(ss_d0), ClusterLimitsPrinter(nss_d0));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(),
      alpha, ClusterLimitsPrinter(ss_d1), ClusterLimitsPrinter(nss_d1));
  GenomicRegionAggregator(2).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(),
      alpha, ClusterLimitsPrinter(ss_d2), ClusterLimitsPrinter(nss_d2));

  EXPECT_EQ(ss_exp_d0.str(), ss_d0.str());
  EXPECT_EQ(ss_exp_d1.str(), ss_d1.str());
  EXPECT_EQ(ss_exp_d2.str(), ss_d2.str());
  EXPECT_EQ(nss_exp_d0.str(), nss_d0.str());
  EXPECT_EQ(nss_exp_d1.str(), nss_d1.str());
  EXPECT_EQ(nss_exp_d2.str(), nss_d2.str());
}


/**
 * \brief TODO
 */
TEST(GRegAggTest, test_agg_summits_pvals) {
  vector<GenomicRegion> regions;
  vector<GenomicRegion> expect_d0_regions, expect_d1_regions, expect_d2_regions;

  regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 8,  9,  "C1R7",  6, '+'));
  regions.push_back(GenomicRegion("chr1", 9,  10, "C1R8",  8, '+'));
  regions.push_back(GenomicRegion("chr1", 10, 11, "C1R9",  1, '+'));
  // ****
  regions.push_back(GenomicRegion("chr2", 3, 4,  "C2R0",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  regions.push_back(GenomicRegion("chr2", 5, 6,  "C2R2",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 6, 7,  "C2R3",  7, '+'));
  regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  regions.push_back(GenomicRegion("chr2", 8, 9,  "C2R5",  5, '+'));
  regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // p-values
  const double pvs[17] = {0.02,  0.0005, 0.001, 0.001, 0.05,
                          0.002, 0.001,  0.05,  0.05,  0.05,
                          0.002, 0.001,  0.002, 0.2,   0.001,
                          0.2,   0.005};
  const vector<double> pvals(pvs, pvs+17);

  // expected regions with cluster distance of 0
  expect_d0_regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 3,  4,  "C2R0",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 4,  5,  "C2R1",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 5,  6,  "C2R2",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 7,  8,  "C2R4",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 9,  10, "C2R6",  2, '+'));


  // expected regions with cluster distance of 1
  expect_d1_regions.push_back(GenomicRegion("chr1", 2, 3,  "C1R1",  2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr1", 7, 8,  "C1R6",  4, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // expected regions with cluster distance of 2
  expect_d2_regions.push_back(GenomicRegion("chr1", 2, 3, "C1R1", 2, '+'));
  expect_d2_regions.push_back(GenomicRegion("chr2", 4, 5, "C2R1", 4, '+'));

  // expected p-values
  const double d0_apvals[10] = {0.0005, 0.001, 0.001, 0.002, 0.001,
                                0.002,  0.001, 0.002, 0.001, 0.005};
  const double d1_apvals[5] =  {0.0005,  0.001, 0.001, 0.001, 0.005};
  const double d2_apvals[2] =  {0.0005,  0.001};
  vector<double> expect_d0_pvals(d0_apvals, d0_apvals + 10),
                 expect_d1_pvals(d1_apvals, d1_apvals + 5),
                 expect_d2_pvals(d2_apvals, d2_apvals + 2);

  stringstream ss_exp_d0, ss_exp_d1, ss_exp_d2;
  for (size_t i=0; i<expect_d0_regions.size(); i++)
    ss_exp_d0 << expect_d0_regions[i] << "\t" << expect_d0_pvals[i] << endl;
  for (size_t i=0; i<expect_d1_regions.size(); i++)
    ss_exp_d1 << expect_d1_regions[i] << "\t" << expect_d1_pvals[i] << endl;
  for (size_t i=0; i<expect_d2_regions.size(); i++)
    ss_exp_d2 << expect_d2_regions[i] << "\t" << expect_d2_pvals[i] << endl;

  stringstream ss_d0, ss_d1, ss_d2;
  double alpha = 0.01;
  GenomicRegionAggregator(0).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), alpha,
      ClusterSummitPrinter(ss_d0, ClusterSummitPrinter::CLUSTER_MIN_SCORE));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), alpha,
      ClusterSummitPrinter(ss_d1, ClusterSummitPrinter::CLUSTER_MIN_SCORE));
  GenomicRegionAggregator(2).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), alpha,
      ClusterSummitPrinter(ss_d2, ClusterSummitPrinter::CLUSTER_MIN_SCORE));

  EXPECT_EQ(ss_exp_d0.str(), ss_d0.str());
  EXPECT_EQ(ss_exp_d1.str(), ss_d1.str());
  EXPECT_EQ(ss_exp_d2.str(), ss_d2.str());
}



/**
 * \brief TODO
 */
TEST(GRegAggTest, test_agg_summits_pvals_covars) {
  vector<GenomicRegion> regions;
  vector<GenomicRegion> expect_d0_regions, expect_d1_regions, expect_d2_regions;
  regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 8,  9,  "C1R7",  6, '+'));
  regions.push_back(GenomicRegion("chr1", 9,  10, "C1R8",  8, '+'));
  regions.push_back(GenomicRegion("chr1", 10, 11, "C1R9",  1, '+'));
  // ****
  regions.push_back(GenomicRegion("chr2", 3, 4,  "C2R0",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  regions.push_back(GenomicRegion("chr2", 5, 6,  "C2R2",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 6, 7,  "C2R3",  7, '+'));
  regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  regions.push_back(GenomicRegion("chr2", 8, 9,  "C2R5",  5, '+'));
  regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // p-values
  const double pvs[17] = {0.02,  0.0005, 0.001, 0.001, 0.05,
                          0.002, 0.001,  0.05,  0.05,  0.05,
                          0.002, 0.001,  0.002, 0.2,   0.001,
                          0.2,   0.005};
  const vector<double> pvals(pvs, pvs+17);

  // covariates
  const double c1[17] = {1,2,5,2,7, 1,2,3,4,10, 1,1,2,5,7, 1,1};
  const double c2[17] = {2,2,7,1,5, 2,2,1,9,11, 2,1,2,7,8, 7,3};
  vector< vector<double> > cvs;
  cvs.push_back(vector<double>(c1,c1+17));
  cvs.push_back(vector<double>(c2,c2+17));

  // expected regions with cluster distance of 0
  expect_d0_regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 3,  4,  "C2R0",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 4,  5,  "C2R1",  4, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 5,  6,  "C2R2",  1, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 7,  8,  "C2R4",  2, '+'));
  expect_d0_regions.push_back(GenomicRegion("chr2", 9,  10, "C2R6",  2, '+'));

  // expected regions with cluster distance of 1
  expect_d1_regions.push_back(GenomicRegion("chr1", 2, 3,  "C1R1",  2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr1", 7, 8,  "C1R6",  4, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  expect_d1_regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // expected regions with cluster distance of 2
  expect_d2_regions.push_back(GenomicRegion("chr1", 2, 3, "C1R1", 2, '+'));
  expect_d2_regions.push_back(GenomicRegion("chr2", 4, 5, "C2R1", 4, '+'));

  // expected p-values
  const double d0_apvals[10] = {0.0005, 0.001, 0.001, 0.002, 0.001,
                                0.002,  0.001, 0.002, 0.001, 0.005};
  const double d1_apvals[5] =  {0.0005,  0.001, 0.001, 0.001, 0.005};
  const double d2_apvals[2] =  {0.0005,  0.001};
  vector<double> expect_d0_pvals(d0_apvals, d0_apvals + 10),
                 expect_d1_pvals(d1_apvals, d1_apvals + 5),
                 expect_d2_pvals(d2_apvals, d2_apvals + 2);

  // expected covariate values
  const double d0_acvars_c1[10] = {2, 5, 2, 1, 2, 1, 1, 2, 7, 1};
  const double d1_acvars_c1[5] = {2, 2, 1, 7, 1};
  const double d2_acvars_c1[2] = {2, 1};
  const double d0_acvars_c2[10] = {2,7,1,2,2,2,1,2,8,3};
  const double d1_acvars_c2[5] = {2, 2, 1, 8, 3};
  const double d2_acvars_c2[2] = {2, 1};
  vector<double> expect_d0_acvars_c1(d0_acvars_c1, d0_acvars_c1+10);
  vector<double> expect_d1_acvars_c1(d1_acvars_c1, d1_acvars_c1+5);
  vector<double> expect_d2_acvars_c1(d2_acvars_c1, d2_acvars_c1+2);
  vector<double> expect_d0_acvars_c2(d0_acvars_c2, d0_acvars_c2+10);
  vector<double> expect_d1_acvars_c2(d1_acvars_c2, d1_acvars_c2+5);
  vector<double> expect_d2_acvars_c2(d2_acvars_c2, d2_acvars_c2+2);

  stringstream ss_exp_d0, ss_exp_d1, ss_exp_d2;
  for (size_t i=0; i<expect_d0_regions.size(); i++)
    ss_exp_d0 << expect_d0_regions[i] << "\t" << expect_d0_pvals[i] << "\t"
              << expect_d0_acvars_c1[i] << "\t" << expect_d0_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d1_regions.size(); i++)
    ss_exp_d1 << expect_d1_regions[i] << "\t" << expect_d1_pvals[i] << "\t"
              << expect_d1_acvars_c1[i] << "\t" << expect_d1_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d2_regions.size(); i++)
    ss_exp_d2 << expect_d2_regions[i] << "\t" << expect_d2_pvals[i] << "\t"
              << expect_d2_acvars_c1[i] << "\t" << expect_d2_acvars_c2[i] << endl;

  vector<vector<double>::const_iterator> begins, ends;
  begins.push_back(cvs[0].begin());
  begins.push_back(cvs[1].begin());
  ends.push_back(cvs[0].end());
  ends.push_back(cvs[1].end());


  stringstream ss_d0, ss_d1, ss_d2;
  double alpha = 0.01;
  GenomicRegionAggregator(0).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      ClusterSummitPrinter(ss_d0, ClusterSummitPrinter::CLUSTER_MIN_SCORE));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      ClusterSummitPrinter(ss_d1, ClusterSummitPrinter::CLUSTER_MIN_SCORE));
  GenomicRegionAggregator(2).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      ClusterSummitPrinter(ss_d2, ClusterSummitPrinter::CLUSTER_MIN_SCORE));

  EXPECT_EQ(ss_exp_d0.str(), ss_d0.str());
  EXPECT_EQ(ss_exp_d1.str(), ss_d1.str());
  EXPECT_EQ(ss_exp_d2.str(), ss_d2.str());
}


/**
 * \brief TODO
 */
/*TEST(GRegAggTest, testFlankingPrinter) {
  vector<GenomicRegion> regions;
  vector<GenomicRegion> expect_d0_s1_regions, expect_d1_s1_regions;
  vector<GenomicRegion> expect_d0_s2_regions;
  vector<GenomicRegion> expect_d1_padded_summits;
  regions.push_back(GenomicRegion("chr1", 1,  2,  "C1R0",  1, '+'));
  regions.push_back(GenomicRegion("chr1", 2,  3,  "C1R1",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  regions.push_back(GenomicRegion("chr1", 7,  8,  "C1R6",  4, '+'));
  regions.push_back(GenomicRegion("chr1", 8,  9,  "C1R7",  6, '+'));
  regions.push_back(GenomicRegion("chr1", 9,  10, "C1R8",  8, '+'));
  regions.push_back(GenomicRegion("chr1", 10, 11, "C1R9",  1, '+'));
  // ****
  regions.push_back(GenomicRegion("chr2", 3, 4,  "C2R0",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 4, 5,  "C2R1",  4, '+'));
  regions.push_back(GenomicRegion("chr2", 5, 6,  "C2R2",  1, '+'));
  regions.push_back(GenomicRegion("chr2", 6, 7,  "C2R3",  7, '+'));
  regions.push_back(GenomicRegion("chr2", 7, 8,  "C2R4",  2, '+'));
  regions.push_back(GenomicRegion("chr2", 8, 9,  "C2R5",  5, '+'));
  regions.push_back(GenomicRegion("chr2", 9, 10, "C2R6",  2, '+'));

  // p-values
  const double pvs[17] = {0.002,  0.0005, 0.02,  0.02,  0.05,
                          0.02,  0.001,  0.0005,  0.05,  0.05,
                          0.002, 0.1,    0.02,  0.2,   0.02,
                          0.2,   0.005};
  const vector<double> pvals(pvs, pvs+17);

  // covariates
  const double c1[17] = {1,2,5,2,7, 1,2,3,4,10, 1,1,2,5,7, 1,1};
  const double c2[17] = {2,2,7,1,5, 2,2,1,9,11, 2,1,2,7,8, 7,3};
  vector< vector<double> > cvs;
  cvs.push_back(vector<double>(c1,c1+17));
  cvs.push_back(vector<double>(c2,c2+17));

  // expected regions with distance of 0 from sig clusters and size 1
  expect_d0_s1_regions.push_back(GenomicRegion("chr1", 3,  4,  "C1R2",  5, '+'));
  expect_d0_s1_regions.push_back(GenomicRegion("chr1", 6,  7,  "C1R5",  2, '+'));
  expect_d0_s1_regions.push_back(GenomicRegion("chr2", 4,  5,  "C2R1",  4, '+'));
  expect_d0_s1_regions.push_back(GenomicRegion("chr2", 8,  9,  "C2R5",  5, '+'));
  // expected regions with distance of 1 from sig clusters and size 1
  expect_d1_s1_regions.push_back(GenomicRegion("chr1", 4,  5,  "C1R3",  2, '+'));
  expect_d1_s1_regions.push_back(GenomicRegion("chr1", 5,  6,  "C1R4",  4, '+'));
  expect_d1_s1_regions.push_back(GenomicRegion("chr2", 5,  6,  "C2R2",  1, '+'));
  expect_d1_s1_regions.push_back(GenomicRegion("chr2", 7,  8,  "C2R4",  2, '+'));
  // expected regions with distance of 0 from sig clusters and size 2
  expect_d0_s2_regions.push_back(GenomicRegion("chr1", 3,  5,  "X",  (2+5)/(double)2, '+'));
  expect_d0_s2_regions.push_back(GenomicRegion("chr1", 5,  7,  "X",  (4+2)/(double)2, '+'));
  expect_d0_s2_regions.push_back(GenomicRegion("chr2", 4,  6,  "X",  (1+4)/(double)2, '+'));
  expect_d0_s2_regions.push_back(GenomicRegion("chr2", 7,  9,  "X",  (2+5)/(double)2, '+'));
  // expected regions for padded sumits
  expect_d1_padded_summits.push_back(GenomicRegion("chr1", 1, 4,  "C1R1", 2, '+'));
  expect_d1_padded_summits.push_back(GenomicRegion("chr1", 7, 10, "C1R7", 6, '+'));
  expect_d1_padded_summits.push_back(GenomicRegion("chr2", 2, 5,  "C2R0", 1, '+'));
  expect_d1_padded_summits.push_back(GenomicRegion("chr2", 8, 11, "C2R6", 2, '+'));

  // expected p-values
  const double d0_s1_apvals[4] = {0.02, 0.02, 0.1, 0.2};
  vector<double> expect_d0_s1_pvals(d0_s1_apvals, d0_s1_apvals + 4);
  const double d1_s1_apvals[4] = {0.02, 0.05, 0.02, 0.02};
  vector<double> expect_d1_s1_pvals(d1_s1_apvals, d1_s1_apvals + 4);
  const double d0_s2_apvals[4] = {0.02, (0.05+0.02)/(double)2,
                                  (0.02+0.1)/(double)2, (0.02+0.2)/(double)2};
  vector<double> expect_d0_s2_pvals(d0_s2_apvals, d0_s2_apvals + 4);
  const double d1_padded_pvals[4] = {0.0005, 0.0005, 0.002, 0.005};
  vector<double> expect_d1_padded_pvals(d1_padded_pvals, d1_padded_pvals+4);

  // expected covariate values
  const double d0_s1_acvars_c1[4] = {5, 1, 1, 1};
  const double d0_s1_acvars_c2[4] = {7, 2, 1, 7};
  vector<double> expect_d0_s1_acvars_c1(d0_s1_acvars_c1, d0_s1_acvars_c1+4);
  vector<double> expect_d0_s1_acvars_c2(d0_s1_acvars_c2, d0_s1_acvars_c2+4);
  const double d1_s1_acvars_c1[4] = {2, 7, 2, 7};
  const double d1_s1_acvars_c2[4] = {1, 5, 2, 8};
  vector<double> expect_d1_s1_acvars_c1(d1_s1_acvars_c1, d1_s1_acvars_c1+4);
  vector<double> expect_d1_s1_acvars_c2(d1_s1_acvars_c2, d1_s1_acvars_c2+4);
  const double d0_s2_acvars_c1[4] = {(2+5)/(double)2, (7+1)/(double)2,
                                     (2+1)/(double)2, (7+1)/(double)2};
  const double d0_s2_acvars_c2[4] = {(1+7)/(double)2, (5+2)/(double)2,
                                     (2+1)/(double)2, (8+7)/(double)2};
  vector<double> expect_d0_s2_acvars_c1(d0_s2_acvars_c1, d0_s2_acvars_c1+4);
  vector<double> expect_d0_s2_acvars_c2(d0_s2_acvars_c2, d0_s2_acvars_c2+4);
  const double d1_padded_cvars_c1[4] = {2, 3, 1, 1};
  const double d1_padded_cvars_c2[4] = {2, 1, 2, 3};
  vector<double> expect_d1_padded_cvars_c1(d1_padded_cvars_c1, d1_padded_cvars_c1 + 4);
  vector<double> expect_d1_padded_cvars_c2(d1_padded_cvars_c2, d1_padded_cvars_c2 + 4);

  stringstream ss_exp_d0_s1, ss_exp_d1_s1, ss_exp_d0_s2, ss_exp_d1_padded;
  for (size_t i=0; i<expect_d0_s1_regions.size(); i++)
    ss_exp_d0_s1 << expect_d0_s1_regions[i] << "\t" << expect_d0_s1_pvals[i] << "\t"
                 << expect_d0_s1_acvars_c1[i] << "\t" << expect_d0_s1_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d1_s1_regions.size(); i++)
    ss_exp_d1_s1 << expect_d1_s1_regions[i] << "\t" << expect_d1_s1_pvals[i] << "\t"
                 << expect_d1_s1_acvars_c1[i] << "\t" << expect_d1_s1_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d0_s2_regions.size(); i++)
    ss_exp_d0_s2 << expect_d0_s2_regions[i] << "\t" << expect_d0_s2_pvals[i] << "\t"
                 << expect_d0_s2_acvars_c1[i] << "\t" << expect_d0_s2_acvars_c2[i] << endl;
  for (size_t i=0; i<expect_d1_padded_summits.size(); i++)
    ss_exp_d1_padded << expect_d1_padded_summits[i] << "\t" << expect_d1_padded_pvals[i] << "\t"
                     << expect_d1_padded_cvars_c1[i] << "\t" << expect_d1_padded_cvars_c2[i] << endl;

  vector<vector<double>::const_iterator> begins, ends;
  begins.push_back(cvs[0].begin());
  begins.push_back(cvs[1].begin());
  ends.push_back(cvs[0].end());
  ends.push_back(cvs[1].end());

  stringstream dummy_sig, ss_d0_s1, ss_d1_s1, ss_d0_s2, ss_d1_padded;
  double alpha = 0.01;
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      ClusterSummitPrinter(dummy_sig, ClusterSummitPrinter::CLUSTER_MIN_SCORE),
      FlankingPrinter(ss_d0_s1, 0, 1));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      ClusterSummitPrinter(dummy_sig, ClusterSummitPrinter::CLUSTER_MIN_SCORE),
      FlankingPrinter(ss_d1_s1, 1, 1));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      ClusterSummitPrinter(dummy_sig, ClusterSummitPrinter::CLUSTER_MIN_SCORE),
      FlankingPrinter(ss_d0_s2, 0, 2));
  GenomicRegionAggregator(1).aggregate(regions.begin(),
      regions.end(), pvals.begin(), pvals.end(), begins, ends, alpha,
      PaddedClusterSummitPrinter(ss_d1_padded, ClusterSummitPrinter::CLUSTER_MIN_SCORE,1),
      FlankingPrinter(dummy_sig, 0, 2));

  EXPECT_EQ(ss_exp_d0_s1.str(), ss_d0_s1.str());
  EXPECT_EQ(ss_exp_d1_s1.str(), ss_d1_s1.str());
  EXPECT_EQ(ss_exp_d0_s2.str(), ss_d0_s2.str());
  EXPECT_EQ(ss_exp_d1_padded.str(), ss_d1_padded.str());
}*/


