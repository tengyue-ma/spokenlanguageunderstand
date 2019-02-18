#! /bin/bash

cd crf_feat
./CrfFeatExtractor_UnitTest.sh

cd ../me_feat
./MEFeatExtractor_UnitTest.sh

cd ../onehot_feat
./OneHotFeatExtractor_UnitTest.sh

cd ../open_domain_feat
./OpenDomainFeatExtractor_UnitTest.sh

