 ###############################################################
 # 
 # Copyright 2011 Red Hat, Inc. 
 # 
 # Licensed under the Apache License, Version 2.0 (the "License"); you 
 # may not use this file except in compliance with the License.  You may 
 # obtain a copy of the License at 
 # 
 #    http://www.apache.org/licenses/LICENSE-2.0 
 # 
 # Unless required by applicable law or agreed to in writing, software 
 # distributed under the License is distributed on an "AS IS" BASIS, 
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and 
 # limitations under the License. 
 # 
 ############################################################### 

condor_selective_glob("StateMachine.*;HAD.*;Utils.*" HADSrcs)

condor_exe(condor_had "${HADSrcs}" ${C_SBIN} "${PCRE_FOUND};${CONDOR_LIBS}" ON)

condor_selective_glob("AbstractReplicatorStateMachine.*;ReplicatorStateMachine.*;HAD_Version.*;Utils.*;Replication.*;FilesOperations.*" RepSrcs)

condor_exe(condor_replication "${RepSrcs}" ${C_SBIN} "${PCRE_FOUND};${CONDOR_LIBS}" OFF)

condor_selective_glob("BaseReplicaTransferer.*;UploadReplicaTransferer.*;DownloadReplicaTransferer.*;Utils.*;Transferer.*;FilesOperations.*" TransferSrcs)

condor_exe(condor_transferer "${TransferSrcs}" ${C_LIBEXEC} "${PCRE_FOUND};${CONDOR_LIBS}" OFF)

