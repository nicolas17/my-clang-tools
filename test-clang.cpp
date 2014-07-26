#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang::ast_matchers;
using namespace clang::tooling;

#define DUMP(x) llvm::errs() << "\e[1m" << #x << " = \e[m\n"; (x)->dump();
#define PRINT(x) llvm::errs() << #x << "\t= " << (x) << "\n";

DeclarationMatcher pmfMatcher =
    anyOf(
        fieldDecl(hasType(memberPointerType())).bind("decl"),
        varDecl(hasType(memberPointerType())).bind("decl")
    );

class MatchHandler: public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult& result) {
        if (const clang::ValueDecl* decl = result.Nodes.getNodeAs<clang::ValueDecl>("decl")) {
            clang::QualType qtype = decl->getType();
            assert(qtype.getTypePtr()->isMemberPointerType());
            const clang::MemberPointerType* mpt = static_cast<const clang::MemberPointerType*>(qtype.getTypePtr());
            const clang::Type* clazz = mpt->getClass();

            if (clazz->isIncompleteType()) {
                llvm::errs() << "Pointer to member using class with only forward declaration!\n";
                decl->dump();
            } else if (clazz->isRecordType()) {
                assert(clazz->getAsCXXRecordDecl());
                if (result.SourceManager->getFileOffset(decl->getLocation()) <
                    result.SourceManager->getFileOffset(clazz->getAsCXXRecordDecl()->getLocation())) {
                    llvm::errs() << "Pointer declared before class defined!\n";
                    decl->dump();
                }
            }

            //llvm::errs() << "\n---\n\n";
        }
    }
};

int main(int argc, const char **argv) {
    CommonOptionsParser optionsParser(argc, argv);

    ClangTool tool(optionsParser.getCompilations(),
                   optionsParser.getSourcePathList());

    MatchHandler handler;
    MatchFinder finder;
    finder.addMatcher(pmfMatcher, &handler);

    return tool.run(newFrontendActionFactory(&finder));
}

